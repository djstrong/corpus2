/*
    Copyright (C) 2010 Tomasz Śniatowski, Adam Radziszewski
    Part of the libcorpus2 project

    This program is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.

    This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. 

    See the LICENSE.CORPUS2, LICENSE.POLIQARP, COPYING.LESSER and COPYING files for more details.
*/

#include "conllwriter.h"
#include <libpwrutils/foreach.h>
#include <boost/algorithm/string.hpp>
#include <algorithm>


namespace Corpus2 {

bool ConllWriter::registered = TokenWriter::register_writer<ConllWriter>("conll");
const std::string ConllWriter::SUPERPOS_ATTR("superpos");

ConllWriter::ConllWriter(std::ostream& os, const Tagset& tagset,
		const string_range_vector& params)
	: TokenWriter(os, tagset, params)
{
	myTagset=tagset;
	// check if the tagset contains 'superpos' attribute

	idx_t superpos_attr = myTagset.get_attribute_index(SUPERPOS_ATTR);
	if (superpos_attr == -1)
	{
		throw Corpus2Error("Tagset " + myTagset.name() +
						   " contains no 'superpos' attribute"
						   " (required by CONLL format)");
	}
	// ensure that the 'superpos' attribute is obligatory and first
	// for each of the gram. classes defined

	for (idx_t pos = 0; pos < myTagset.pos_count(); ++pos) {
		const std::vector<bool> req_attrs = myTagset.get_pos_required_attributes(pos);
		// superpos_attr is the index of 'superpos' attr
		// this index should be within range of required attributes for pos
		// the attrubite should be marked as required
		if ((idx_t)req_attrs.size() <= superpos_attr)
		{
			throw Corpus2Error("Tagset " + myTagset.name() +
							   " should define 'superpos' attribute for each"
							   " grammatical class (req. by CONLL writer)");
		}
		if (!req_attrs[superpos_attr])
		{
			throw Corpus2Error("Tagset " + myTagset.name() +
							   " should define 'superpos' attribute"
							   " as REQUIRED for each class"
							   " (req. by CONLL writer)");
		}
		// ensure that no attribute comes before superpos
		if (tagset.get_pos_attributes(pos)[0] != superpos_attr)
		{
			throw Corpus2Error("Tagset " + myTagset.name() +
							   " should define 'superpos' attribute"
							   " as the FIRST one for each class"
							   " (req. by CONLL writer)");
		}
	}
}

ConllWriter::~ConllWriter()
{
	finish();
}

void ConllWriter::write_token(const Token &t)
{
	const Lexeme &lex = t.get_preferred_lexeme(myTagset);
	os() << t.orth_utf8() << "\t" << lex.lemma_utf8() << "\t";

	// get lower-case tag representation
	std::string tagstr = myTagset.tag_to_string(lex.tag());
	std::transform(tagstr.begin(), tagstr.end(), tagstr.begin(), ::tolower);

	// ugly, but should work: split the lower tag repr on colons
	std::vector<std::string> segs;
	boost::split(segs, tagstr, boost::is_any_of(":"));

	// now write each part of the split string and pad the non-existent
	// attributes with _
	// (ctr has asserted that after the obligatory gram. class comes
	// 'superpos' attribute, so it is safe to assume there are always
	// at least 2 segments)
	os() << segs[1] << "\t" << segs[0] << "\t";
	if(segs.size() > 2)
	{
		size_t i;
		for(i = 2; i < segs.size() - 1; i++)
		{
			os() << segs[i] <<"|";
		}
		os() << segs[i] << "\t_\t_\t_\t_";
	}
	else
	{
		os()<< "_\t_\t_\t_\t_";
	}
}

void ConllWriter::write_sentence(const Sentence& s)
{
	int i=1;
	foreach (const Token* t, s.tokens()) {
		os()<<i<<"\t";
		write_token(*t);
		os()<<"\n";
		i++;
	}
	os()<<"\n";
}

void ConllWriter::write_chunk(const Chunk &c)
{
	foreach (const Sentence::ConstPtr& s, c.sentences()) {
		write_sentence(*s);
	}
}

void ConllWriter::do_header()
{

}

void ConllWriter::do_footer()
{
}

} /* end ns Corpus2 */
