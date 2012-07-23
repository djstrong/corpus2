#!/usr/bin/python
# -*- coding: utf-8 -*-

# Copyright (C) 2012 Adam Radziszewski.
# This program is free software; you can redistribute and/or modify it
# under the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 3 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.
#
# See the LICENCE and COPYING files for more details

descr = """%prog [options] CHUNKED REF CHAN_NAME

Reads the two chunk-annotated corpora: CHUNKED (chunker output) and REF
(reference annotation / gold standard). Outputs precision and recall values
for the following settings:
1. chunk recognition (counting as hit when exactly same tokens)
2. chunk + head recognition (as above + heads placed on the same tokens)
3. head recognition alone (only head position is compared)

NOTE: this script treats discontinuous chunks as whole annotations.
"""


from optparse import OptionParser
import sys
import corpus2

class Stats:
	def __init__(self):
		self.ch_chunks = 0
		self.ref_chunks = 0
		self.chunk_hits = 0
		self.head_hits = 0
		self.both_hits = 0
	
	def update(self, ch_annots, ref_annots):
		self.ch_chunks += len(ch_annots)
		self.ref_chunks += len(ref_annots)
		# sort by left border
		ch = dict([(min(ann.indices), ann) for ann in ch_annots])
		ref = dict([(min(ann.indices), ann) for ann in ref_annots])
		ch_idx = ref_idx = 0
		maybe_hits = set(ch).intersection(ref)
		for idx in maybe_hits:
			if list(ch[idx].indices) == list(ref[idx].indices):
				self.chunk_hits += 1
				if ch[idx].head_index == ref[idx].head_index:
					self.both_hits += 1
		# now compare head indices only
		ch = set(ann.head_index for ann in ch_annots)
		ref = set(ann.head_index for ann in ref_annots)
		self.head_hits += len(ch.intersection(ref))
	
	def dump_prf(self, name, hits):
		p = 0.0 if self.ch_chunks == 0 else 100.0 * hits / self.ch_chunks
		r = 0.0 if self.ref_chunks == 0 else 100.0 * hits / self.ref_chunks
		f = 0.0 if p + r == 0.0 else 2.0 * p * r / (p + r)
		print '%s\t%.2f\t%.2f\t%.2f' % (name, p, r, f)
	
	def dump(self, verbosity = 2):
		if verbosity > 1:
			print 'CHU chunks\t%d' % self.ch_chunks
			print 'REF chunks\t%d' % self.ref_chunks
			print 'Chunk hits\t%d' % self.chunk_hits
			print 'Head  hits\t%d' % self.head_hits
			print 'Ch+Hd hits\t%d' % self.both_hits
		if verbosity > 0:
			self.dump_prf('Chunk P,R,F', self.chunk_hits)
			self.dump_prf('Heads P,R,F', self.head_hits)
			self.dump_prf('Ch+Hd P,R,F', self.both_hits)


def get_annots(sent, chan_name):
	# wrap the sentence as an AnnotatedSentence
	annots = []
	asent = corpus2.AnnotatedSentence.wrap_sentence(sent)
	if asent.has_channel(chan_name):
		chan = asent.get_channel(chan_name)
		ann_vec = chan.make_annotation_vector()
		for ann in ann_vec:
			assert ann.head_index in ann.indices
			annots.append(ann)
	return annots


def go():
	parser = OptionParser(usage=descr)
	parser.add_option('-i', '--input-format', type='string', action='store',
		dest='input_format', default='ccl',
		help='set the input format; default: ccl')
	parser.add_option('-t', '--tagset', type='string', action='store',
		dest='tagset', default='nkjp',
		help='set the tagset used in input; default: nkjp')
	parser.add_option('-q', '--quiet', action='store_false',
		default=True, dest='verbose')
	(options, args) = parser.parse_args()
	
	if len(args) != 3:
		sys.stderr.write('No args. See --help\n')
		sys.exit(1)
	
	ch_path, ref_path, chan_name = args
	tagset = corpus2.get_named_tagset(options.tagset)
	ch_rdr = corpus2.TokenReader.create_path_reader(
			options.input_format, tagset, ch_path)
	ref_rdr = corpus2.TokenReader.create_path_reader(
			options.input_format, tagset, ref_path)
	
	stats = Stats()
	
	while True:
		# iterate over paragraphs (note that they are called "chunks" here)
		ref_chunk = ref_rdr.get_next_chunk()
		ch_chunk = ch_rdr.get_next_chunk()
		assert (not ref_chunk) == (not ch_chunk), 'corpora of different length'
		
		if not ref_chunk:
			break # end of input
		
		# process each sentence separately
		for ch_sent, ref_sent in zip(ch_chunk.sentences(), ref_chunk.sentences()):
			assert ch_sent.size() == ref_sent.size()
			ch_annots = get_annots(ch_sent, chan_name)
			ref_annots = get_annots(ref_sent, chan_name)
			stats.update(ch_annots, ref_annots)
	stats.dump(int(options.verbose) + 1)

if __name__ == '__main__':
	go()
