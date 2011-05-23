/*
	Copyright (C) 2010 Tomasz Śniatowski, Adam Radziszewski
	Part of the libcorpus2 project

	This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.

	This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.

	See the LICENSE and COPYING files for more details.
*/

#include <libcorpus2/io/iob-chan.h>
#include <libpwrutils/foreach.h>
#include <libcorpus2/ann/annotatedsentence.h>

#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <fstream>
#include <boost/algorithm/string/split.hpp>

namespace Corpus2 {

bool IobChanWriter::registered = TokenWriter::register_writer<IobChanWriter>(
		"iob-chan", "nowarn");

IobChanWriter::IobChanWriter(std::ostream& os, const Tagset& tagset,
		const string_range_vector& params)
	: TokenWriter(os, tagset, params), warn_on_no_lexemes_(true)
{
	foreach (const string_range& param, params) {
		std::string p = boost::copy_range<std::string>(param);
		if (p == "nowarn") {
			warn_on_no_lexemes_ = false;
		}
	}
}

void IobChanWriter::write_token(const Token& t)
{
	os() << t.orth_utf8();
	if (t.lexemes().empty()) {
		if (warn_on_no_lexemes_) {
			std::cerr << "No lexemes for token!";
		}
	} else {
		const Lexeme& pref = t.get_preferred_lexeme(tagset());
		os() << "\t";
		write_tag(pref.tag());
	}
	os() << "\n";
}

void IobChanWriter::write_sentence(const Sentence& s)
{
	const AnnotatedSentence* ann = dynamic_cast<const AnnotatedSentence*>(&s);
	for (size_t idx = 0; idx < s.size(); ++idx) {
		const Token* t = s.tokens()[idx];
		os() << t->orth_utf8();
		if (t->lexemes().empty()) {
			if (warn_on_no_lexemes_) {
				std::cerr << "No lexemes for token!";
			}
		} else {
			const Lexeme& pref = t->get_preferred_lexeme(tagset());
			os() << "\t";
			write_tag(pref.tag());
		}
		if (ann) {
			bool first = true;
			foreach (const AnnotatedSentence::chan_map_t::value_type& v, ann->all_channels()) {
				if (!first) {
					os() << ",";
				}
				os() << v.first << "-";
				os() << Corpus2::IOB::to_string(v.second.get_iob_at(idx));
				first = false;
			}
		}
		os() << "\n";
	}
	os() << "\n";
}

void IobChanWriter::write_chunk(const Chunk& c)
{
	foreach (const Sentence::ConstPtr& s, c.sentences()) {
		write_sentence(*s);
	}
}

void IobChanWriter::write_tag(const Tag& tag)
{
	os() << tagset().tag_to_string(tag);
}


bool IobChanReader::registered = TokenReader::register_reader<IobChanReader>("iob-chan",
	"ign,loose,strict,no_set_disamb");


IobChanReader::IobChanReader(const Tagset& tagset, std::istream& is)
	: BufferedSentenceReader(tagset), is_(&is), disamb_(true)
{
}

IobChanReader::IobChanReader(const Tagset& tagset, const std::string& filename)
	: BufferedSentenceReader(tagset), is_(), disamb_(true)
{
	is_owned_.reset(new std::ifstream(filename.c_str(), std::ifstream::in));
	if (!this->is_owned_->good()) {
		throw Corpus2Error("File not found!");
	}
	else {
		this->is_ = is_owned_.get();
	}
}

Sentence::Ptr IobChanReader::actual_next_sentence()
{
	std::string line;
	AnnotatedSentence::Ptr s;
	typedef boost::split_iterator<std::string::const_iterator> string_split_iterator;

	while (is().good()) {
		std::getline(is(), line);
		if (line.empty()) {
			return s;
		}
		std::vector<std::string> spl;
		boost::algorithm::split(spl, line, boost::is_any_of("\t"));
		if (spl.size() != 4) {
			std::cerr << "Invalid line: " << line << "\n";
		} else {
			const std::string& orth = spl[0];
			const std::string& lemma = spl[0];
			const std::string& tag_string = spl[1];
			Tag tag = parse_tag(tag_string);
			Token* t = new Token();
			t->set_orth(UnicodeString::fromUTF8(orth));
			t->set_wa(PwrNlp::Whitespace::Space);
			t->add_lexeme(Lexeme(UnicodeString::fromUTF8(lemma), tag));
			if (disamb_) {
				t->lexemes().back().set_disamb(true);
			}
			s->append(t);
			const std::string& cline = line;
			for (string_split_iterator value_it = boost::make_split_iterator(
					cline, boost::token_finder(boost::is_any_of(",")));
					value_it != string_split_iterator();
					++value_it) {

			}
		}
	}
	return s;
}

void IobChanReader::set_option(const std::string &option)
{
	if (option == "no_set_disamb") {
		disamb_ = false;
	} else {
		BufferedSentenceReader::set_option(option);
	}
}

std::string IobChanReader::get_option(const std::string &option) const
{
	if (option == "no_set_disamb") {
		return !disamb_ ? option : "";
	}
	return BufferedSentenceReader::get_option(option);
}

} /* end ns Corpus2 */
