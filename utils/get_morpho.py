#!/usr/bin/python
# -*- coding: utf-8 -*-

# Copyright (C) 2013 Adam Radziszewski.
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

descr = """%prog [options] CORPUS OUTDATAFILE

Gathers morphological data from the given corpus.

The extracted morphological data will be saved to OUTDATAFILE in tab-delimited
format (compliant with Maca analysers).
NOTE: the morphological data will not be compacted. To do so, you may use
tabclean.py script from Maca repository.
"""

import sys, codecs
from optparse import OptionParser
import corpus2

class Analyser:
	"""Morphological dictionary resulting from reading a corpus.
	Provides consume function that updates the data with the given token."""
	def __init__(self, tagset, case_sensitive):
		self.tagset = tagset
		self.unk_tag = self.tagset.make_ign_tag()
		self.get_form = (lambda form: form) if case_sensitive else (lambda form: form.lower())
		self.data = {}
	
	def _add_one(self, ready_form, lex):
		"""Adds tag and lemma from the given lexeme."""
		if ready_form not in self.data:
			self.data[ready_form] = set()
		self.data[ready_form].add(
			(unicode(lex.lemma()), self.tagset.tag_to_string(lex.tag()))
		)
	
	def consume(self, tok):
		ready_form = self.get_form(unicode(tok.orth()))
		for lex in tok.lexemes():
			if lex.tag() != self.unk_tag:
				self._add_one(ready_form, lex)
	
	def write(self, output):
		for form in sorted(self.data):
			entries = sorted(self.data[form])
			for lemma, tag in entries:
				output.write(u'%s\t%s\t%s\n' % (form, lemma, tag))
	
	def save(self, fname):
		output = codecs.open(fname, 'wb', 'utf-8')
		self.write(output)
		output.close()

def get_morpho(options, corpname, outfname):
	tagset = corpus2.get_named_tagset(options.tagset)
	anal = Analyser(tagset, options.case_sens)
	rdr = corpus2.TokenReader.create_path_reader(options.input_format, tagset, corpname)
	while True:
		tok = rdr.get_next_token()
		if not tok:
			break
		anal.consume(tok)
	del rdr
	anal.save(outfname)

def go():
	parser = OptionParser(usage=descr)
	parser.add_option('-i', '--input-format', type='string', action='store',
		dest='input_format', default='xces',
		help='set the input format; default: xces')
	parser.add_option('-t', '--tagset', type='string', action='store',
		dest='tagset', default='nkjp',
		help='set the tagset used in input; default: nkjp')
	parser.add_option('-s', '--sep', type='string', action='store',
		dest='sep', default='\t',
		help='set the separator used in morpho file; default: tab character')
	parser.add_option('-c', '--case-sensitive', action='store_true', default=False, dest='case_sens')
	parser.add_option('-q', '--quiet', action='store_false', default=True, dest='verbose')
	(options, args) = parser.parse_args()
	
	if len(args) != 2:
		print 'You need to provide an input corpus and output path'
		print
		parser.print_help()
		sys.exit(1)
	
	corpname = args[0]
	outfname = args[1]
	
	get_morpho(options, corpname, outfname)

if __name__ == '__main__':
	go()
