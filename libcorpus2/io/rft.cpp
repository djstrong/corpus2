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

#include <libcorpus2/io/rft.h>

#include <libpwrutils/foreach.h>

#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <fstream>

namespace Corpus2 {

bool RftWriter::registered = TokenWriter::register_writer<RftWriter>(
		"rft", "mbt,nowarn");

RftWriter::RftWriter(std::ostream& os, const Tagset& tagset,
		const string_range_vector& params)
	: TokenWriter(os, tagset, params), warn_on_no_lexemes_(true)
	, mbt_dialect_(false)
{
	foreach (const string_range& param, params) {
		std::string p = boost::copy_range<std::string>(param);
		if (p == "nowarn") {
			warn_on_no_lexemes_ = false;
		} else if (p == "mbt") {
			mbt_dialect_ = true;
		}

	}
}

void RftWriter::write_token(const Token& t)
{
	os() << t.orth_utf8() << "\t";
	if (t.lexemes().empty()) {
		if (warn_on_no_lexemes_) {
			std::cerr << "No lexemes for token!";
		}
	} else {
		const Lexeme& pref = t.get_preferred_lexeme(tagset());
		std::string tag_str = tagset().tag_to_no_opt_string(pref.tag());
		os () << (mbt_dialect_
			? tag_str // when MBT-compliant, suppress colon substitution
			: boost::algorithm::replace_all_copy(tag_str, ":", "."));
	}
	os() << "\n";
}

void RftWriter::write_sentence(const Sentence& s)
{
	foreach (const Token* t, s.tokens()) {
		write_token(*t);
	}
	if (mbt_dialect_) {
		os() << "<utt>";
	}
	os() << "\n";
}

void RftWriter::write_chunk(const Chunk& c)
{
	foreach (const Sentence::ConstPtr& s, c.sentences()) {
		write_sentence(*s);
	}
}

bool RftReader::registered = TokenReader::register_reader<RftReader>("rft",
	"ign,loose,strict,set_disamb,mbt");


RftReader::RftReader(const Tagset& tagset, std::istream& is, bool disamb,
		bool mbt_dialect)
	: BufferedSentenceReader(tagset), is_(&is), disamb_(disamb)
	, mbt_dialect_(mbt_dialect)
{
}

RftReader::RftReader(const Tagset& tagset, const std::string& filename, bool disamb,
		bool mbt_dialect)
	: BufferedSentenceReader(tagset), is_(), disamb_(disamb)
	, mbt_dialect_(mbt_dialect)
{
	is_owned_.reset(new std::ifstream(filename.c_str(), std::ifstream::in));
	if (this->is_owned_->bad()) {
		throw Corpus2Error("File not found!");
	}
	else {
		this->is_ = is_owned_.get();
	}
}

Sentence::Ptr RftReader::actual_next_sentence()
{
	std::string line;
	Sentence::Ptr s;
	while (is().good()) {
		std::getline(is(), line);
		if (line.empty()
			|| (mbt_dialect_ && line.find_first_of("<utt>") == 0)) { // TODO: check
			return s;
		} else {
			size_t tab = line.find('\t');
			if (tab == line.npos || tab == 0 || (tab == line.size() - 1)) {
				std::cerr << "Invalid line: " << line << "\n";
			} else {
				std::string orth = line.substr(0, tab);
				std::string tag_string = line.substr(tab + 1);
				if (!mbt_dialect_) {
					boost::algorithm::replace_all(tag_string, ".", ":");
				}
				Tag tag = parse_tag(tag_string);
				Token* t = new Token();
				t->set_orth(UnicodeString::fromUTF8(orth));
				t->set_wa(PwrNlp::Whitespace::Space);
				if (!s) {
					s = boost::make_shared<Sentence>();
					t->set_wa(PwrNlp::Whitespace::Newline);
				}
				t->add_lexeme(Lexeme(t->orth(), tag));
				if (disamb_) {
					t->lexemes().back().set_disamb(true);
				}
				s->append(t);
			}
		}
	}
	return s;
}

void RftReader::set_option(const std::string &option)
{
	if (option == "mbt") {
		mbt_dialect_ = true;
	} else if (option == "set_disamb") {
		disamb_ = true;
	} else {
		BufferedSentenceReader::set_option(option);
	}
}

std::string RftReader::get_option(const std::string &option)
{
	if (option == "mbt") {
		return mbt_dialect_ ? option : "";
	} else if (option == "set_disamb") {
		return disamb_ ? option : "";
	}
	return BufferedSentenceReader::get_option(option);
}

} /* end ns Corpus2 */
