#!/usr/bin/python
# -*- coding: utf-8 -*-

from optparse import OptionParser
import sys
import corpus2
from StringIO import StringIO

descr = """%prog [options] TAGDFOLD1 ... REFFOLD1 ...

Evaluates tagging of tagged corpus consisting of possibly several folds using
the given reference corpus (gold standard). The subsequent folds should match.
"""

changelog = """
* respect_spaces
* report POS hits also
* comments
* fix debug strings
* skip non-disamb tokens as Pantera artefacts
* higher frac precision in output
* extract measures to functions for averaging
* averaging over folds
* separate stats for unknown forms
"""

def text(tok_seq, respect_spaces):
	"""Extracts text from a sequence of tokens. If respect_spaces, will append
	spaces between tokens where no no-space markers present."""
	buff = StringIO()
	nonfirst = False
	for tok in tok_seq:
		if nonfirst and respect_spaces and tok.after_space():
			buff.write(' ')
		buff.write(tok.orth_utf8().decode('utf-8'))
		nonfirst = True
	return buff.getvalue()
		
def next_tok(rdr):
	while True:
		tok = rdr.get_next_token()
		if not tok:
			return tok
		if tok.has_disamb_lexeme():
			return tok

def tok_seqs(rdr_here, rdr_there, respect_spaces, verbose_mode, debug_mode):
	"""Generates subsequent aligned token sequences from the two readers.
	Alignment is performed on the text level. Shortest token sequences with
	matching text are generated.
	"""
	
	tok_here = next_tok(rdr_here)
	tok_there = next_tok(rdr_there)
	
	assert tok_here and tok_there, 'no input'
	
	buff_here = [tok_here]
	buff_there = [tok_there]
	
	LIMIT = 30
	
	num_iter = 0
	
	while True:
		num_iter += 1
		if num_iter % 10000 == 0: print num_iter, 'iterations...'
		if len(buff_here) > LIMIT or len(buff_there) > LIMIT:
			raise IOError('limit exceeded')
		
		text_here = text(buff_here, respect_spaces)
		text_there = text(buff_there, respect_spaces)
		
		if debug_mode:
			print '%s (%d) v. %s(%d)' % (text_here, len(text_here),
			text_there, len(text_there))
		
		if len(text_here) == len(text_there):
			# already aligned
			if text_here != text_there:
				# the same text length but different chars
				# won't recover, flush it as it is
				print 'WARNING: mismatch', text_here, '|', text_there
			yield (buff_here, buff_there)
			buff_here = []
			buff_there = []
			tok_here = next_tok(rdr_here)
			tok_there = next_tok(rdr_there)
			if not tok_here or not tok_there:
				assert not tok_here and not tok_there, 'one ended prematurely'
				break
			buff_here = [tok_here]
			buff_there = [tok_there]
		elif len(text_here) < len(text_there):
			# try fetching here
			tok_here = next_tok(rdr_here)
			assert tok_here, 'first ended prematurely'
			buff_here.append(tok_here)
		else: # len(text_here) > len(text_there):
			# try fetching there
			tok_there = next_tok(rdr_there)
			assert tok_there, 'second ended prematurely'
			buff_there.append(tok_there)
	
	if buff_here or buff_there:
		assert buff_here and buff_there
		yield (buff_here, buff_there)



class TokComp:
	"""Creates a tagger evaluation comparator. The comparator reads two
	annotated texts: tagged text (output of a tagger being evaluated) and
	reference tagging (the gold standard). Most of the figures reported concern
	whether a reference token has got some sort of coverage in the tagger
	output.
	
	Differences in segmentation are treated by aligning tokens textually,
	i.e. shortest matching sequences are extracted and then compared.
	A heuristic approach is used: differences in tokenisation are not penalised
	when those differences concern either punctuation-only sequences, or both
	sequences contain exactly one non-punctuation token. In the first case,
	each of the reference token belonging to such a sequence is treated as full
	hit. In the latter case, the whole reference sequence is judged by the one
	non-punct token.
	
	punc_tag is a string representation of tag used for punctuation.
	unk_tag is a string representation of tag used for unknown words.
	Set expand_optional to True if ommission of optional attribute
	values should be treated as multiple tags, each with a different
	variant of the value."""
	def __init__(self, tagset, punc_tag, unk_tag,
		expand_optional, debug = False):
		self.tagset = tagset
		self.punc_tag = punc_tag
		self.unk_tag = unk_tag
		self.expand_optional = expand_optional
		self.debug = debug
		self.ref_toks = 0 # all tokens in ref corpus
		self.ref_toks_amb = 0 # tokens subjected to segmentation ambiguities
		# tokens not subjected to s.a. that contribute to weak correctness (WC)
		self.ref_toks_noamb_weak_hit = 0
		# not subjected to s.a. that contribute to strong correctness (SC)
		self.ref_toks_noamb_strong_hit = 0
		# not subjected to s.a. that contribute to SC on POS level
		self.ref_toks_noamb_pos_strong_hit = 0 # exactly same sets of POSes
		# tokens subjected to s.a. that contribute to WC
		self.ref_toks_amb_weak_hit = 0
		# tokens subjected to s.a. that contribute to SC
		self.ref_toks_amb_strong_hit = 0
		# tokens subjected to s.a. that contribute to SC on POS level
		self.ref_toks_amb_pos_strong_hit = 0
		# tokens subjected to s.a. that were weakly hit thanks to punc-only
		self.ref_toks_amb_weak_punc_hit = 0
		# tokens subjected to s.a. that were weakly hit thanks to punc-around
		self.ref_toks_amb_weak_puncplus_hit = 0
		# ref toks presumably unknown (unk tag among ref tok tags)
		self.ref_toks_unk = 0
		# ref toks that contribute to WC, amb + noamb
		self.ref_toks_unk_weak_hit = 0
		# ref toks that contribute to SC on POS level, amb + noamb
		self.ref_toks_unk_pos_strong_hit = 0
		
		
		self.tag_toks = 0 # all tokens in tagger output
		self.tag_toks_amb = 0 # tokens in tagger output subjected to s.a.
	
	def is_punc(self, tok):
		tok_tags = set([self.tagset.tag_to_string(lex.tag()) for lex in tok.lexemes() if lex.is_disamb()])
		return tok_tags == set([self.punc_tag])
	
	def is_unk(self, tok):
		tok_tags = [self.tagset.tag_to_string(lex.tag()) for lex in tok.lexemes()]
		return self.unk_tag in tok_tags
	
	def tagstrings_of_token(self, tok):
		"""Returns a set of strings, corresponding to disamb tags
		found in the token. If expand_optional, multiple tags may be
		produced from single-disamb tokens when some optional attributes
		are unspecified."""
		tags = [lex.tag() for lex in tok.lexemes() if lex.is_disamb()]
		if self.expand_optional:
			# create multivalue tag wherever a value of optional attr is unspec
			tags = [self.tagset.expand_optional_attrs(tag) for tag in tags]
			# now expand multivalue tags to singular tags
			newtags = []
			for tag in tags:
				newtags.extend(self.tagset.split_tag(tag))
			tags = newtags
			if self.debug:
				print ' + '.join(self.tagset.tag_to_string(tag) for tag in tags)
		return set(self.tagset.tag_to_string(tag) for tag in tags)
	
	def cmp_toks(self, tok1, tok2):
		"""Returns a tuple: (hitlevel, poshitlevel), where hitlevel concerns
		whole tag comparison, while poshitleve concerns POS only. Bot levels
		are integers: 2 if both tokens have the same sets of disamb tags
		(POS-es), 1 if they intersect, 0 otherwise."""
		tok1_tags = self.tagstrings_of_token(tok1)
		tok2_tags = self.tagstrings_of_token(tok2)
		tok1_pos = set(t.split(':', 1)[0] for t in tok1_tags)
		tok2_pos = set(t.split(':', 1)[0] for t in tok2_tags)
		pos_hit = (tok1_pos == tok2_pos)
		
		taghitlevel, poshitlevel = 0, 0
		
		if tok1_tags == tok2_tags:
			taghitlevel = 2
		elif tok1_tags.intersection(tok2_tags):
			taghitlevel = 1
		
		if tok1_pos == tok2_pos:
			poshitlevel = 2
		elif tok1_pos.intersection(tok2_pos):
			poshitlevel = 1
		
		return (taghitlevel, poshitlevel)
	
	def update(self, tag_seq, ref_seq):
		self.tag_toks += len(tag_seq)
		self.ref_toks += len(ref_seq)
		
		unk_tokens = sum(self.is_unk(ref_tok) for ref_tok in ref_seq)
		self.ref_toks_unk += unk_tokens
		
		# first variant: no segmentation mess
		if len(tag_seq) == 1 and len(ref_seq) == 1:
			tagval, posval = self.cmp_toks(tag_seq[0], ref_seq[0])
			if tagval > 0:
				self.ref_toks_noamb_weak_hit += len(ref_seq)
				self.ref_toks_unk_weak_hit += unk_tokens
			if tagval == 2:
				self.ref_toks_noamb_strong_hit += len(ref_seq)
			if posval == 2:
				self.ref_toks_noamb_pos_strong_hit += len(ref_seq)
				self.ref_toks_unk_pos_strong_hit += unk_tokens
			
			if self.debug: print '\t\tnormal', tagval, posval
		else:
			self.ref_toks_amb += len(ref_seq)
			self.tag_toks_amb += len(tag_seq)
			all_punc_ref = all(self.is_punc(tok) for tok in ref_seq)
			all_punc_tag = all(self.is_punc(tok) for tok in tag_seq)
			if all_punc_ref and all_punc_tag:
				# second variant: PUNC v. PUNC gives hit
				#print '-'.join(tokx.orth_utf8() for tokx in tag_seq)
				#print '-'.join(tokx.orth_utf8() for tokx in ref_seq)
				self.ref_toks_amb_weak_punc_hit += len(ref_seq)
				self.ref_toks_amb_weak_hit += len(ref_seq)
				self.ref_toks_amb_strong_hit += len(ref_seq)
				self.ref_toks_amb_pos_strong_hit += len(ref_seq)
				self.ref_toks_unk_weak_hit += unk_tokens # unlikely that unk_tokens > 0
				self.ref_toks_unk_pos_strong_hit += unk_tokens # as above
				if self.debug: print '\t\tpunc hit, ref len', len(ref_seq)
			else:
				nonpunc_ref = [tok for tok in ref_seq if not self.is_punc(tok)]
				nonpunc_tag = [tok for tok in tag_seq if not self.is_punc(tok)]
				
				tagval, posval = 0, 0
				if len(nonpunc_ref) == len(nonpunc_tag) == 1:
					tagval, posval = self.cmp_toks(nonpunc_tag[0], nonpunc_ref[0])
				if tagval > 0:
					self.ref_toks_amb_weak_hit += len(ref_seq)
					self.ref_toks_amb_weak_puncplus_hit += len(ref_seq)
					self.ref_toks_unk_weak_hit += unk_tokens
					if self.debug: print '\t\tpuncPLUS weak hit, ref len', len(ref_seq)
				if tagval == 2:
					self.ref_toks_amb_strong_hit += len(ref_seq)
					if self.debug: print '\t\tpuncPLUS strong hit, ref len', len(ref_seq)
				if tagval == 0:
					# miss
					if self.debug: print '\t\tMISS, ref len', len(ref_seq)
				if posval == 2:
					self.ref_toks_amb_pos_strong_hit += len(ref_seq)
					self.ref_toks_unk_pos_strong_hit += unk_tokens
	
	def weak_lower_bound(self):
		"""Returns weak correctness percentage counting only hits where
		segmentation did not change. That is, lower bound of real WC."""
		return 100.0 * self.ref_toks_noamb_weak_hit / self.ref_toks
	
	def strong_lower_bound(self):
		"""Returns strong correctness percentage counting only hits where
		segmentation did not change. That is, lower bound of real SC."""
		return 100.0 * self.ref_toks_noamb_strong_hit / self.ref_toks
	
	def weak_corr(self):
		"""Returns weak correctness, counting changes in segmentation
		as failure unless they concern punctuation (see above for two
		rules)."""
		all_weak_hits = self.ref_toks_amb_weak_hit + self.ref_toks_noamb_weak_hit
		return 100.0 * all_weak_hits / self.ref_toks
	
	def strong_corr(self):
		"""As above but SC."""
		all_strong_hits = self.ref_toks_amb_strong_hit + self.ref_toks_noamb_strong_hit
		return 100.0 * all_strong_hits / self.ref_toks
	
	def pos_strong_corr(self):
		"""POS-only SC."""
		all_pos_strong_hits = self.ref_toks_amb_pos_strong_hit + self.ref_toks_noamb_pos_strong_hit
		return 100.0 * all_pos_strong_hits / self.ref_toks
	
	def weak_upper_bound(self):
		"""Upper bound for weak correctness, i.e. counting every reference
		token subjected to segmentation change as hit."""
		upper_weak_hits = self.ref_toks_noamb_weak_hit + self.ref_toks_amb
		return 100.0 * upper_weak_hits / self.ref_toks
	
	def strong_upper_bound(self):
		"""Upper bound for SC."""
		upper_strong_hits = self.ref_toks_noamb_strong_hit + self.ref_toks_amb
		return 100.0 * upper_strong_hits / self.ref_toks

	def unk_weak_corr(self):
		return 100.0 * self.ref_toks_unk_weak_hit / self.ref_toks_unk

	def unk_pos_strong_corr(self):
		return 100.0 * self.ref_toks_unk_pos_strong_hit / self.ref_toks_unk
	
	def dump(self):
		print '----'
		print 'REF-toks\t%d' % self.ref_toks
		print 'REF-toks-unk\t%d\t%.4f%%' % (self.ref_toks_unk, 100.0 * self.ref_toks_unk / self.ref_toks)
		print 'TAGGER-toks\t%d' % self.tag_toks
		print 'REF-amb-toks\t%d\t%.4f%%' % (self.ref_toks_amb, 100.0 * self.ref_toks_amb / self.ref_toks)
		print 'TAGGER-amb-toks\t%d\t%.4f%%' % (self.tag_toks_amb, 100.0 * self.tag_toks_amb / self.tag_toks)
		print
		print 'REF-weak-hits-noamb (lower bound)\t%d\t%.4f%%' % \
			(self.ref_toks_noamb_weak_hit, self.weak_lower_bound())
		print 'REF-strong-hits-noamb (lower bound)\t%d\t%.4f%%' % \
			(self.ref_toks_noamb_strong_hit, self.strong_lower_bound())
		print
		print 'REF-weak-hits-amb (heur recover)\t%d\t%.4f%%' % \
			(self.ref_toks_amb_weak_hit, \
			100.0 * self.ref_toks_amb_weak_hit / self.ref_toks)
		print 'REF-weak-punc-amb-hits (heur1 recover)\t%d\t%.4f%%' % \
			(self.ref_toks_amb_weak_punc_hit, \
			100.0 * self.ref_toks_amb_weak_punc_hit / self.ref_toks)
		print 'REF-weak-puncPLUS-amb-hits (heur2 recover)\t%d\t%.4f%%' % \
			(self.ref_toks_amb_weak_puncplus_hit, \
			100.0 * self.ref_toks_amb_weak_puncplus_hit / self.ref_toks)
		print 'REF-strong-POS-hits-amb (heur recover)\t%d\t%.4f%%' % \
			(self.ref_toks_amb_pos_strong_hit, \
			100.0 * self.ref_toks_amb_pos_strong_hit / self.ref_toks)
		print
		
		print 'REF-weak-hits-all (heur)\t%d\t%.4f%%' % \
			(self.ref_toks_amb_weak_hit + self.ref_toks_noamb_weak_hit, \
			self.weak_corr())
		
		print 'REF-strong-hits-all (heur)\t%d\t%.4f%%' % \
			(self.ref_toks_amb_strong_hit + self.ref_toks_noamb_strong_hit, \
			self.strong_corr())
		
		print 'REF-POS-strong-hits-all (heur)\t%d\t%.4f%%' % \
			(self.ref_toks_amb_pos_strong_hit + self.ref_toks_noamb_pos_strong_hit, \
			self.pos_strong_corr())
		print
		# all amb as hits
		print 'REF-weak-hits-hitamb (upper bound)\t%d\t%.4f%%' % \
			(self.ref_toks_noamb_weak_hit + self.ref_toks_amb, \
			self.weak_upper_bound())
		
		print 'REF-strong-hits-hitamb (upper bound)\t%d\t%.4f%%' % \
			(self.ref_toks_noamb_strong_hit + self.ref_toks_amb, \
			self.strong_upper_bound())

def go():
	parser = OptionParser(usage=descr)
	parser.add_option('-i', '--input-format', type='string', action='store',
		dest='input_format', default='xces',
		help='set the input format; default: xces')
	parser.add_option('-t', '--tagset', type='string', action='store',
		dest='tagset', default='nkjp',
		help='set the tagset used in input; default: nkjp')
	parser.add_option('-q', '--quiet', action='store_false', default=True, dest='verbose')
	parser.add_option('-p', '--punc-tag', type='string', action='store',
		dest='punc_tag', default='interp',
		help='set the tag used for punctuation; default: interp')
	parser.add_option('-u', '--unk-tag', type='string', action='store',
		dest='unk_tag', default='ign',
		help='set the tag used for unknown forms; default: ign')
	parser.add_option('-k', '--keep-optional', action='store_false',
		default=True, dest='expand_optional',
		help='do not expand unspecified optional attributes to multiple tags')
	parser.add_option('-s', '--ignore-spaces', action='store_false',
		default=True, dest='respect_spaces',
		help='ignore spaces between tokens when comparing')
	parser.add_option('-d', '--debug', action='store_true', dest='debug_mode')
	(options, args) = parser.parse_args()
	
	if len(args) < 2 or len(args) % 2 != 0:
		print 'You need to provide a series of tagged folds and a coresponding'
		print 'series of reference folds.'
		print 'See --help for details.'
		print
		sys.exit(1)
	
	tagset = corpus2.get_named_tagset(options.tagset)
	
	num_folds = len(args) / 2
	
	weak_lower_bound = 0.0
	weak_upper_bound = 0.0
	weak = 0.0
	strong_pos = 0.0

	unk_weak = 0.0
	unk_strong_pos = 0.0
	
	for fold_idx in range(num_folds):
		tag_fn = args[fold_idx] # filename of tagged fold @ fold_idx
		ref_fn = args[fold_idx + num_folds] # ... reference fold @ fold_idx
		if options.verbose:
			print '### FOLD %2d: %s (tag) v. %s (ref)' % ((fold_idx + 1), tag_fn, ref_fn)
		tag_rdr = corpus2.TokenReader.create_path_reader(options.input_format, tagset, tag_fn)
		ref_rdr = corpus2.TokenReader.create_path_reader(options.input_format, tagset, ref_fn)
		
		res = TokComp(tagset, options.punc_tag, options.unk_tag,
			options.expand_optional, options.debug_mode)
		for tag_seq, ref_seq in tok_seqs(tag_rdr, ref_rdr, options.respect_spaces, options.verbose, options.debug_mode):
			res.update(tag_seq, ref_seq)
		if options.verbose:
			res.dump()
		weak_lower_bound += res.weak_lower_bound()
		weak_upper_bound += res.weak_upper_bound()
		weak += res.weak_corr()
		strong_pos += res.pos_strong_corr()
		unk_weak += res.unk_weak_corr()
		unk_strong_pos += res.unk_pos_strong_corr()
	
	print 'AVG weak corr lower bound\t%.4f%%' % (weak_lower_bound / num_folds)
	print 'AVG weak corr upper bound\t%.4f%%' % (weak_upper_bound / num_folds)
	print 'AVG weak corr (heur)\t%.4f%%' % (weak / num_folds)
	print 'AVG POS strong corr\t%.4f%%' % (strong_pos / num_folds)
	print 'AVG UNK weak corr (heur)\t%.4f%%' % (unk_weak / num_folds)
	print 'AVG UNK POS strong corr\t%.4f%%' % (unk_strong_pos / num_folds)
	

if __name__ == '__main__':
	go()
