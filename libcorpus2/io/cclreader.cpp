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

#include <libcorpus2/io/cclreader.h>
#include <libcorpus2/io/sax.h>
#include <libpwrutils/foreach.h>
#include <libxml++/libxml++.h>
#include <libxml2/libxml/parser.h>
#include <boost/make_shared.hpp>
#include <libcorpus2/ann/annotatedsentence.h>
#include <cstdlib>
#include <fstream>

namespace Corpus2 {

class CclReaderImpl : public BasicSaxParser
{
public:
	CclReaderImpl(const Tagset& tagset,
		std::deque< boost::shared_ptr<Chunk> >& obuf,
		bool disamb_only, bool disamb_sh);

	~CclReaderImpl();

protected:
	void on_start_element(const Glib::ustring & name,
			const AttributeList& attributes);
	void on_end_element(const Glib::ustring & name);

	void finish_sentence();

	const Tagset& tagset_;

	enum state_t { XS_NONE, XS_CHUNK, XS_SENTENCE, XS_TOK, XS_ANN, XS_ORTH, XS_LEX,
			XS_LEMMA, XS_TAG, XS_REL };
	state_t state_;

	bool chunkless_;

	bool out_of_chunk_;

	PwrNlp::Whitespace::Enum wa_;

	Glib::ustring sbuf_;

	Token* tok_;

	boost::shared_ptr<AnnotatedSentence> sent_;

	std::string ann_chan_;

	bool ann_head_;

	typedef std::map<std::string, int> token_ann_t;

	token_ann_t token_anns_;

	std::set<std::string> token_ann_heads_;

	boost::shared_ptr<Chunk> chunk_;

	std::deque< boost::shared_ptr<Chunk> >& obuf_;

	bool disamb_only_;

	bool disamb_sh_;
};

CclReader::CclReader(const Tagset& tagset, std::istream& is,
		bool disamb_only, bool disamb_sh)
	: BufferedChunkReader(tagset),
	impl_(new CclReaderImpl(tagset, chunk_buf_, disamb_only, disamb_sh))
{
	this->is_ = &is;
}

CclReader::CclReader(const Tagset& tagset, const std::string& filename, bool disamb_only, bool disamb_sh)
	: BufferedChunkReader(tagset),
	impl_(new CclReaderImpl(tagset, chunk_buf_, disamb_only, disamb_sh))
{
	this->is_owned_.reset(new std::ifstream(filename.c_str(), std::ifstream::in));

	if (this->is_owned_->bad()) {
		throw Corpus2Error("File not found!");
	}
	else {
		this->is_ = is_owned_.get();
	}
}

CclReader::~CclReader()
{
}

void CclReader::ensure_more()
{
	static const int BUFSIZE=1024;
	while (chunk_buf_.empty() && is().good()) {
		unsigned char buf[BUFSIZE+1];
		is().read(reinterpret_cast<char*>(buf), BUFSIZE);
		impl_->parse_chunk_raw(buf, is().gcount());
		if (is().eof()) {
			impl_->finish_chunk_parsing();
		}
	}
}

CclReaderImpl::CclReaderImpl(const Tagset& tagset,
		std::deque< boost::shared_ptr<Chunk> >& obuf,
		bool disamb_only, bool disamb_sh)
	: BasicSaxParser()
	, tagset_(tagset), state_(XS_NONE), chunkless_(false), out_of_chunk_(false)
	, wa_(PwrNlp::Whitespace::Newline)
	, sbuf_(), tok_(NULL), sent_(), chunk_(), obuf_(obuf)
	, disamb_only_(disamb_only), disamb_sh_(disamb_sh)
{
}

CclReaderImpl::~CclReaderImpl()
{
	delete tok_;
}

void CclReaderImpl::on_start_element(const Glib::ustring &name,
		const AttributeList& attributes)
{
	if (name == "chunk") {
		std::string type;
		foreach (const Attribute& a, attributes) {
			if (a.name == "type") {
				type = a.value;
			}
		}
		if (type == "s") {
			throw XcesError("Trying to parse XCES as CCL (<chunk type=\"s\">)");
		} else if (state_ == XS_NONE) {
			chunk_ = boost::make_shared<Chunk>();
			state_ = XS_CHUNK;
			foreach (const Attribute& a, attributes) {
				chunk_->set_attribute(a.name, a.value);
			}
		} else if (state_ == XS_CHUNK) {
			throw XcesError("Nested <chunk>");
		} else {
			throw XcesError("Unexpected <chunk>");
		}
	} else if (state_ == XS_CHUNK && name == "sentence") {
		state_ = XS_SENTENCE;
		sent_ = boost::make_shared<AnnotatedSentence>();
	} else if (state_ == XS_SENTENCE && name == "tok") {
		state_ = XS_TOK;
		tok_ = new Token();
		tok_->set_wa(wa_);
		wa_ = PwrNlp::Whitespace::Space;
		token_anns_.clear();
		token_ann_heads_.clear();
	} else if (state_ == XS_TOK && name == "orth") {
		state_ = XS_ORTH;
		grab_characters_ = true;
		clear_buf();
	} else if (state_ == XS_TOK && name == "ann") {
		state_ = XS_ANN;
		grab_characters_ = true;
		clear_buf();
		ann_chan_ = "";
		ann_head_ = false;
		foreach (const Attribute& a, attributes) {
			if (a.name == "chan") {
				ann_chan_ = a.value;
			} else if (a.name == "head" && a.value == "1") {
				ann_head_ = true;
			}
		}
		if (ann_chan_.empty()) {
			throw XcesError("<ann> with no channel name");
		}
	} else if (state_ == XS_TOK && name == "lex") {
		assert(tok_ != NULL);
		bool is_disamb = false;
		foreach (const Attribute& a, attributes) {
			if (a.name == "disamb" && a.value == "1") {
				is_disamb = true;
			}
		}
		if (!disamb_only_ || is_disamb) {
			tok_->add_lexeme(Lexeme());
			tok_->lexemes().back().set_disamb(is_disamb);
			state_ = XS_LEX;
		}
	} else if (state_ == XS_LEX && name == "base") {
		state_ = XS_LEMMA;
		grab_characters_ = true;
		clear_buf();
	} else if (state_ == XS_LEX && name == "ctag") {
		state_ = XS_TAG;
		grab_characters_ = true;
		clear_buf();
	} else if (name == "ns") {
		wa_ = PwrNlp::Whitespace::None;
	} else if (name == "tok" && state_ == XS_NONE) {
		std::cerr << "Warning: out-of-chunk token, assuming sentence start on line ";
		std::cerr << this->context_->input->line << "\n";
		chunkless_ = true;
		out_of_chunk_ = true;
		chunk_ = boost::make_shared<Chunk>();
		sent_ = boost::make_shared<AnnotatedSentence>();
		state_ = XS_TOK;
		tok_ = new Token();
		tok_->set_wa(wa_);
		wa_ = PwrNlp::Whitespace::Space;
	}
}

void CclReaderImpl::finish_sentence()
{
	chunk_->append(sent_);
	sent_.reset();
	if (chunkless_) {
		obuf_.push_back(chunk_);
		chunk_.reset();
		state_ = XS_NONE;
		chunkless_ = false;
	} else {
		state_ = XS_CHUNK;
	}
}

void CclReaderImpl::on_end_element(const Glib::ustring &name)
{
	if (state_ == XS_ORTH && name == "orth") {
		tok_->set_orth(UnicodeString::fromUTF8(get_buf()));
		grab_characters_ = false;
		state_ = XS_TOK;
	} else if (state_ == XS_ANN && name == "ann") {
		std::string buf = get_buf();
		grab_characters_ = false;
		int segid = atoi(buf.c_str());
		if (!sent_->has_channel(ann_chan_)) {
			sent_->create_channel(ann_chan_);
		}
		if (segid > 0) {
			token_anns_.insert(std::make_pair(ann_chan_, segid));
			token_ann_heads_.insert(ann_chan_);
		}
		state_ = XS_TOK;
	} else if (state_ == XS_LEMMA && name == "base") {
		tok_->lexemes().back().set_lemma(UnicodeString::fromUTF8(get_buf()));
		grab_characters_ = false;
		state_ = XS_LEX;
	} else if (state_ == XS_TAG && name == "ctag") {
		Tag tag = tagset_.parse_simple_tag(get_buf(), true);
		tok_->lexemes().back().set_tag(tag);
		grab_characters_ = false;
		state_ = XS_LEX;
	} else if (state_ == XS_LEX && name == "lex") {
		state_ = XS_TOK;
	} else if (state_ == XS_TOK && name == "tok") {
		sent_->append(tok_);
		tok_ = NULL;
		state_ = XS_SENTENCE;
		foreach (const token_ann_t::value_type& v, token_anns_) {
			sent_->get_channel(v.first).set_segment_at(sent_->size() - 1, v.second);
			if (token_ann_heads_.find(v.first) != token_ann_heads_.end()) {
				sent_->get_channel(v.first).set_head_at(sent_->size() - 1, true);
			}
		}
	} else if (state_ == XS_SENTENCE && name == "sentence") {
		finish_sentence();
	} else if (state_ == XS_CHUNK && name == "chunk") {
		obuf_.push_back(chunk_);
		chunk_.reset();
		state_ = XS_NONE;
	}
}

} /* end ns Corpus2 */
