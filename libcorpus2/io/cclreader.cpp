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
#include <libcorpus2/io/xmlreader.h>
#include <libpwrutils/foreach.h>
#include <libxml++/libxml++.h>
#include <libxml2/libxml/parser.h>
#include <boost/make_shared.hpp>
#include <libcorpus2/ann/annotatedsentence.h>
#include <cstdlib>
#include <fstream>

namespace Corpus2 {

class CclReaderImpl : public XmlReader
{
public:
	CclReaderImpl(const Tagset& tagset,
		std::deque< boost::shared_ptr<Chunk> >& obuf,
		bool disamb_only, bool disamb_sh);

	~CclReaderImpl();

protected:
	bool process_start_element(const Glib::ustring & name,
		const AttributeList& attributes);

	bool process_end_element(const Glib::ustring& name);

	void start_chunk(const AttributeList &attributes);

	void start_sentence(const AttributeList &attributes);

	void start_token(const AttributeList &attributes);

	void finish_token();

	static const int STATE_ANN = 901;
	static const int STATE_REL = 902;

	boost::shared_ptr<AnnotatedSentence> ann_sent_;

	std::string ann_chan_;

	bool ann_head_;

	typedef std::map<std::string, int> token_ann_t;

	token_ann_t token_anns_;

	std::set<std::string> token_ann_heads_;
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
	: XmlReader(tagset, obuf)
{
	XmlReader::set_disamb_only(disamb_only);
	XmlReader::set_disamb_sh(disamb_sh);
	sentence_tag_name_ = "sentence";
}

CclReaderImpl::~CclReaderImpl()
{
}

void CclReaderImpl::start_chunk(const AttributeList& attributes)
{
	chunk_ = boost::make_shared<Chunk>();
	std::string type = get_type_from_attributes(attributes);
	if (type == "s") {
		throw XcesError("Trying to parse XCES as CCL (<chunk type=\"s\">)");
	}
	foreach (const Attribute& a, attributes) {
		chunk_->set_attribute(a.name, a.value);
	}
	state_ = STATE_CHUNK;
}



void CclReaderImpl::start_sentence(const AttributeList& /*attributes*/)
{
	ann_sent_ = boost::make_shared<AnnotatedSentence>();
	sent_ = ann_sent_;
	state_ = STATE_SENTENCE;
}


void CclReaderImpl::start_token(const AttributeList& attributes)
{
	XmlReader::start_token(attributes);
	token_anns_.clear();
	token_ann_heads_.clear();
}

bool CclReaderImpl::process_start_element(const Glib::ustring & name,
	const AttributeList& attributes)
{
	if (state_ == STATE_TOK && name == "ann") {
		state_ = STATE_ANN;
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
		return true;
	} else {
		return false;
	}
}

bool CclReaderImpl::process_end_element(const Glib::ustring & name)
{
	if (state_ == STATE_ANN && name == "ann") {
		std::string buf = get_buf();
		grab_characters_ = false;
		int segid = atoi(buf.c_str());
		if (!ann_sent_->has_channel(ann_chan_)) {
			ann_sent_->create_channel(ann_chan_);
		}
		if (segid > 0) {
			token_anns_.insert(std::make_pair(ann_chan_, segid));
			if (ann_head_) {
				token_ann_heads_.insert(ann_chan_);
			}
		}
		state_ = STATE_TOK;
		return true;
	} else {
		return false;
	}
}

void CclReaderImpl::finish_token()
{
	XmlReader::finish_token();
	foreach (const token_ann_t::value_type& v, token_anns_) {
		ann_sent_->get_channel(v.first).set_segment_at(sent_->size() - 1, v.second);
		if (token_ann_heads_.find(v.first) != token_ann_heads_.end()) {
			ann_sent_->get_channel(v.first).set_head_at(sent_->size() - 1, true);
		}
	}
}

void CclReader::set_option(const std::string& option)
{
	if (option == "loose") {
		impl_->set_loose_tag_parsing(true);
	} else if (option == "strict") {
		impl_->set_loose_tag_parsing(false);
	} else if (option == "no_warn_inconsistent") {
		impl_->set_warn_on_inconsistent(false);
	}
}

} /* end ns Corpus2 */
