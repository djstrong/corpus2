#!/usr/bin/python
# -*- coding: utf-8 -*-
descr = """%prog [options] INPUT OUTPUT

Reads input and saves as plain text. By default, paragraphs are separated with
two newlines, sentence division is not marked."""

from optparse import OptionParser
import sys, codecs
import corpus2

def go():
	parser = OptionParser(usage=descr)
	parser.add_option('-i', '--input-format', type='string', action='store',
		dest='input_format', default='xces',
		help='set the input format; default: xces')
	parser.add_option('-t', '--tagset', type='string', action='store',
		dest='tagset', default='nkjp',
		help='set the tagset used in input; default: nkjp')
	parser.add_option('-s', '--sent-sep', type='string', action='store',
		dest='sent_sep', default='',
		help='set the sentence separator; default: (empty)')
	parser.add_option('-p', '--par-sep', type='string', action='store',
		dest='par_sep', default='\n\n',
		help='set the sentence separator; default: (two newlines)')
	(options, args) = parser.parse_args()
	if len(args) != 2:
		print 'Need to provide input and output.'
		print 'See --help for details.'
		print
		sys.exit(1)
	
	fn_input, fn_output = args
	
	with codecs.open(fn_output, 'wb', 'utf-8') as out:
		tagset = corpus2.get_named_tagset(options.tagset)
		rdr = corpus2.TokenReader.create_path_reader(options.input_format, tagset, fn_input)
		first = True
		while True:
			par = rdr.get_next_chunk()
			if options.par_sep:
				first = True # if non-empty par separator, skip pre-spaces
			if not par:
				break
			for sent in par.sentences():
				if options.sent_sep:
					first = True # if non-empty sent sep, skip pre-spaces
				for tok in sent.tokens():
					if not first and tok.after_space():
						out.write(' ')
					out.write(unicode(tok.orth()))
					first = False
				out.write(options.sent_sep)
			out.write(options.par_sep)

if __name__ == '__main__':
	go()
