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

descr = """%prog [options] INPUT OUTDIR

Generates paragraph-wise folds."""

from optparse import OptionParser
import sys, codecs, os
import corpus2

def go():
	parser = OptionParser(usage=descr)
	parser.add_option('-i', '--input-format', type='string', action='store',
		dest='input_format', default='xces',
		help='set the input format; default: xces')
	parser.add_option('-o', '--output-format', type='string', action='store',
		dest='output_format', default='xces',
		help='set the output format; default: xces')
	parser.add_option('-t', '--tagset', type='string', action='store',
		dest='tagset', default='nkjp',
		help='set the tagset used in input; default: nkjp')
	parser.add_option('-f', '--num-folds', type='int', action='store',
		dest='num_folds', default='10',
		help='set the number of folds (default: 10)')
	
	(options, args) = parser.parse_args()
	if len(args) != 2:
		print 'Need to provide input and output.'
		print 'See --help for details.'
		print
		sys.exit(1)
	
	fold_nums = range(options.num_folds)
	fn_input, fold_dir = args
	
	tagset = corpus2.get_named_tagset(options.tagset)
	rdr = corpus2.TokenReader.create_path_reader(options.input_format, tagset, fn_input)
	fold_test = [corpus2.TokenWriter.create_path_writer(
			options.output_format,
			os.path.join(fold_dir, 'test%02d.xml' % (num + 1)), tagset)
			for num in fold_nums]
	fold_train = [corpus2.TokenWriter.create_path_writer(
			options.output_format,
			os.path.join(fold_dir, 'train%02d.xml' % (num + 1)), tagset)
			for num in fold_nums]
	first = True
	fold_now = 0
	while True:
		par = rdr.get_next_chunk()
		if not par:
			break
		fold_test[fold_now].write_chunk(par)
		for other_num in fold_nums:
			if other_num != fold_now:
				fold_train[other_num].write_chunk(par)
		
		fold_now = (fold_now + 1) % options.num_folds
		
	for w in fold_test: w.finish()
	for w in fold_train: w.finish()

if __name__ == '__main__':
	go()
