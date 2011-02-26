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

#include <libcorpus2/io/xmlreader.h>
#include <libpwrutils/foreach.h>
#include <libxml++/libxml++.h>
#include <libxml2/libxml/parser.h>
#include <boost/make_shared.hpp>
#include <fstream>

namespace Corpus2 {


XmlReader::XmlReader(const Tagset& tagset,
		std::deque< boost::shared_ptr<Chunk> >& obuf)
	: BasicSaxParser()
	, tagset_(tagset), state_(STATE_NONE)
	, chunkless_(false), out_of_chunk_(false)
	, wa_(PwrNlp::Whitespace::Newline)
	, sbuf_(), tok_(NULL), sent_(), chunk_(), obuf_(obuf)
	, disamb_only_(false), disamb_sh_(false)
	, warn_on_inconsistent_(true), warn_on_unexpected_(true)
	, loose_tag_parsing_(false)
{
}

XmlReader::~XmlReader()
{
	delete tok_;
}

std::string XmlReader::get_type_from_attributes(const AttributeList& attributes) const
{
	std::string type;
	foreach (const Attribute& a, attributes) {
		if (a.name == "type") {
			type = a.value;
		}
	}
	return type;
}


void XmlReader::on_start_element(const Glib::ustring &name,
		const AttributeList& attributes)
{
	//std::cerr << name << state_ << "\n";
	if (state_ == STATE_NONE && name == "chunk") {
		start_chunk(attributes);
	} else if (state_ == STATE_CHUNK && name == sentence_tag_name_) {
		start_sentence(attributes);
	} else if (state_ == STATE_SENTENCE && name == "tok") {
		start_token(attributes);
	} else if (state_ == STATE_TOK && name == "orth") {
		state_ = STATE_ORTH;
		grab_characters_ = true;
		clear_buf();
	} else if (state_ == STATE_TOK && name == "lex") {
		start_lexeme(attributes);
	} else if (state_ == STATE_LEX && name == "base") {
		state_ = STATE_LEMMA;
		grab_characters_ = true;
		clear_buf();
	} else if (state_ == STATE_LEX && name == "ctag") {
		state_ = STATE_TAG;
		grab_characters_ = true;
		clear_buf();
	} else if (name == "ns") {
		wa_ = PwrNlp::Whitespace::None;
	} else if (state_ == STATE_NONE && name == "tok") {
		if (warn_on_inconsistent_) {
			std::cerr << "Warning: out-of-chunk token, assuming sentence start on line ";
			std::cerr << this->context_->input->line << "\n";
		}
		AttributeList fake;
		start_chunk(fake);
		fake.push_back(Attribute("type", "s"));
		start_sentence(fake);
		chunkless_ = true;
		out_of_chunk_ = true;
		start_token(attributes);
	} else if (state_ == STATE_NONE && name == "cesAna") {
		//nop
	} else if (state_ == STATE_NONE && name == "chunkList") {
		//nop
	} else if (process_start_element(name, attributes)) {
		//nop
	} else if (warn_on_unexpected_) {
		std::cerr << "Unexpected tag <" << name << "> on line ";
		std::cerr << this->context_->input->line << " (" << state_ << ")\n";
	}
}

bool XmlReader::process_start_element(const Glib::ustring &/*name*/,
	const AttributeList &/*attributes*/)
{
	return false;
}

bool XmlReader::process_end_element(const Glib::ustring & /*name*/)
{
	return false;
}


void XmlReader::start_chunk(const AttributeList& attributes)
{
	if (out_of_chunk_) {
		finish_sentence();
		out_of_chunk_ = false;
	}
	std::string type = get_type_from_attributes(attributes);
	chunk_ = boost::make_shared<Chunk>();

	if (type == "s") {
		// top-level chunk is a sentence
		start_sentence(attributes);
		chunkless_ = true;
	} else {
		foreach (const Attribute& a, attributes) {
			chunk_->set_attribute(a.name, a.value);
		}
		state_ = STATE_CHUNK;
	}
}

void XmlReader::start_sentence(const AttributeList &attributes)
{
	std::string type = get_type_from_attributes(attributes);
	if (type != "s") {
		throw XcesError("Sub level <chunk> not type=\"s\"");
	}
	sent_ = boost::make_shared<Corpus2::Sentence>();
	state_ = STATE_SENTENCE;
}

void XmlReader::start_token(const AttributeList &/*attributes*/)
{
	state_ = STATE_TOK;
	tok_ = new Token();
	tok_->set_wa(wa_);
	wa_ = PwrNlp::Whitespace::Space;
}

void XmlReader::start_lexeme(const AttributeList &attributes)
{
	assert(tok_ != NULL);
	bool is_disamb = false;
	if (!disamb_sh_) {
		foreach (const Attribute& a, attributes) {
			if (a.name == "disamb" && a.value == "1") {
				is_disamb = true;
			}
		}
	} else {
		is_disamb = true;
		foreach (const Attribute& a, attributes) {
			if (a.name == "disamb_sh" && a.value == "0") {
				is_disamb = false;
			}
		}
	}
	if (!disamb_only_ || is_disamb) {
		tok_->add_lexeme(Lexeme());
		tok_->lexemes().back().set_disamb(is_disamb);
		state_ = STATE_LEX;
	}
}

void XmlReader::finish_chunk()
{
	assert(chunk_);
	obuf_.push_back(chunk_);
	chunk_.reset();
	state_ = STATE_NONE;
}

void XmlReader::finish_sentence()
{
	assert(chunk_);
	chunk_->append(sent_);
	sent_.reset();
	if (chunkless_) {
		obuf_.push_back(chunk_);
		chunk_.reset();
		state_ = STATE_NONE;
		chunkless_ = false;
	} else {
		state_ = STATE_CHUNK;
	}
}

void XmlReader::finish_token()
{
	assert(sent_);
	sent_->append(tok_);
	tok_ = NULL;
	state_ = STATE_SENTENCE;
}

void XmlReader::on_end_element(const Glib::ustring &name)
{
	//std::cerr << "/" << name << state_ << "\n";
	if (state_ == STATE_ORTH && name == "orth") {
		tok_->set_orth(UnicodeString::fromUTF8(get_buf()));
		grab_characters_ = false;
		state_ = STATE_TOK;
	} else if (state_ == STATE_LEMMA && name == "base") {
		tok_->lexemes().back().set_lemma(UnicodeString::fromUTF8(get_buf()));
		grab_characters_ = false;
		state_ = STATE_LEX;
	} else if (state_ == STATE_TAG && name == "ctag") {
		Tag tag = tagset_.parse_simple_tag(get_buf(),
			loose_tag_parsing_ ? Tagset::ParseLoose : Tagset::ParseDefault);
		tok_->lexemes().back().set_tag(tag);
		grab_characters_ = false;
		state_ = STATE_LEX;
	} else if (state_ == STATE_LEX && name == "lex") {
		state_ = STATE_TOK;
	} else if (state_ == STATE_TOK && name == "tok") {
		finish_token();
	} else if (state_ == STATE_SENTENCE && name == sentence_tag_name_) {
		finish_sentence();
	} else if (state_ == STATE_CHUNK && name == "chunk") {
		finish_chunk();
	} else {
		process_end_element(name);
	}
}

} /* end ns Corpus2 */
