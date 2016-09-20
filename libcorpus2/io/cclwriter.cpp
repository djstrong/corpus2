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

#include <libcorpus2/io/cclwriter.h>
#include <boost/foreach.hpp>
#include <libcorpus2/ann/annotatedsentence.h>
#include <libcorpus2/io/xcescommon.h>
#include <libcorpus2/tokenmetadata.h>

namespace Corpus2 {

bool CclWriter::registered = TokenWriter::register_writer<CclWriter>("ccl",
		"flat,chunk,nochunk,nodisamb,sorttags,split,ws");

CclWriter::CclWriter(std::ostream& os, const Tagset& tagset,
		const string_range_vector& params)
	: XmlWriter(os, tagset, params)
{
	do_header();
}

CclWriter::~CclWriter()
{
	finish();
}

void CclWriter::write_sentence(const Sentence& s)
{
	paragraph_head();
	if (use_indent_) indent_more();
	write_sentence_int(s);
	if (use_indent_) indent_less();
	osi() << "</chunk>\n";
}

void CclWriter::write_sentence_int(const Sentence &s)
{
	const boost::shared_ptr<Sentence> sentence = s.clone_shared();
	const boost::shared_ptr<AnnotatedSentence> ann = AnnotatedSentence::wrap_sentence(sentence);

	std::string id = s.id();
	if (id == "") {
		osi() << "<sentence>\n";
	}
	else {
		osi() << "<sentence id=\"" << id << "\">\n";
	}

	if (use_indent_) indent_more();
	for (size_t idx = 0; idx < s.size(); ++idx) {
		const Token* t = s.tokens()[idx];
		if (ann) {
			token_as_xces_xml_head(os(), *t, use_indent_ ? indent_level() : -1, whitespace_info_);
			if (use_indent_) indent_more();
			token_as_xces_xml_body(os(), tagset(), *t, use_indent_ ? indent_level() : -1, output_disamb_, sort_tags_);
			BOOST_FOREACH(const AnnotatedSentence::chan_map_t::value_type& v, ann->all_channels()) {
				osi() << "<ann chan=\"" << v.first << "\"";
				if (v.second.is_head_at(idx)) {
					os() << " head=\"1\"";
				}
				os() << ">";
				os() << v.second.get_segment_at(idx);
				os() << "</ann>\n";
			}
			boost::shared_ptr<TokenMetaData> md = t->get_metadata();
			if (md) {
				BOOST_FOREACH(const TokenMetaData::attr_map_t::value_type& v, md->attributes()) {
					osi() << "<prop key=\"" << v.first << "\"" << ">";
					encode_xml_entities_into(os(), v.second);
					os() << "</prop>\n";
				}
			}
			if (use_indent_) indent_less();
			osi() << "</tok>\n";
		} else {
			// TODO: currently writing of token metadata is supported only when
			// we've got an AnnotatedSentence.
			XmlWriter::write_token(*t);
		}
	}
	if (use_indent_) indent_less();	osi() << "</sentence>\n";
}

void CclWriter::write_chunk(const Chunk &c)
{
	paragraph_head(c);
	if (use_indent_) indent_more();
	BOOST_FOREACH(const Sentence::ConstPtr& s, c.sentences()) {
		write_sentence_int(*s);
	}
	if (use_indent_) indent_less();
	osi() << "</chunk>\n";
}

void CclWriter::do_header()
{
	XmlWriter::do_header();
	os() << "<!DOCTYPE chunkList SYSTEM \"ccl.dtd\">\n";
	os() << "<chunkList";
	//os() << " xmlns:xlink=\"http://www.w3.org/1999/xlink\"";
	os() << ">\n";
	if (use_indent_) indent_more();
}

void CclWriter::do_footer()
{
	if (use_indent_) indent_less();
	os() << "</chunkList>\n";
}

void CclWriter::paragraph_head()
{
	osi() << "<chunk id=\"ch" << ++cid_ << "\""
		<< " type=\"p\">\n";
}

void CclWriter::paragraph_head(const Chunk& c)
{
	// in CCL format chunks may have at most two attributes:
	// id (unique XML-style id) and type (typically p for paragraphs)
	osi() << "<chunk";
	if (c.has_attribute("id")) {
		const std::string &val = c.get_attribute("id");
		if (!val.empty()) {
			os() << " id=\"" << val << "\"";
		}
	}
	if (c.has_attribute("type")) {
		const std::string &val = c.get_attribute("type");
		if (!val.empty()) {
			os() << " type=\"" << val << "\"";
		}
	}
	os() << ">\n";
}

} /* end ns Corpus2 */
