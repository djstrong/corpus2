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

#include <libcorpus2/io/reader.h>
#include <boost/make_shared.hpp>

namespace Corpus2 {

TokenReader::TokenReader(const Tagset& tagset)
	: tagset_(tagset)
{
}

TokenReader::~TokenReader()
{
}

BufferedChunkReader::BufferedChunkReader(const Tagset& tagset)
	: TokenReader(tagset)
{
}

BufferedChunkReader::~BufferedChunkReader()
{
	foreach (Token* t, token_buf_) {
		delete t;
	}
}

Token* BufferedChunkReader::get_next_token()
{
	bool more = true;
	while (token_buf_.empty() && more) {
		ensure_more();
		Sentence::Ptr s = get_next_sentence();
		if (s) {
			std::copy(s->tokens().begin(), s->tokens().end(),
					std::back_inserter(token_buf_));
			s->release_tokens();
		} else {
			more = false;
		}
	}
	if (token_buf_.empty()) {
		return NULL;
	} else {
		Token* t = token_buf_.front();
		token_buf_.pop_front();
		return t;
	}
}

Sentence::Ptr BufferedChunkReader::get_next_sentence()
{
	bool more = true;
	while (sentence_buf_.empty() && more) {
		ensure_more();
		boost::shared_ptr<Chunk> c = get_next_chunk();
		if (c) {
			std::copy(c->sentences().begin(), c->sentences().end(),
					std::back_inserter(sentence_buf_));
		} else {
			more = false;
		}
	}
	if (sentence_buf_.empty()) {
		return Sentence::Ptr();
	} else {
		Sentence::Ptr s = sentence_buf_.front();
		sentence_buf_.pop_front();
		return s;
	}
}

boost::shared_ptr<Chunk> BufferedChunkReader::get_next_chunk()
{
	ensure_more();
	if (chunk_buf_.empty()) {
		return boost::shared_ptr<Chunk>();
	} else {
		boost::shared_ptr<Chunk> t = chunk_buf_.front();
		chunk_buf_.pop_front();
		return t;
	}
}

BufferedSentenceReader::BufferedSentenceReader(const Tagset& tagset)
	: TokenReader(tagset), chunkify_(true)
	, sentence_buf_(), token_buf_()
{
}

Token* BufferedSentenceReader::get_next_token()
{
	bool more = true;
	while (token_buf_.empty() && more) {
		Sentence::Ptr s = get_next_sentence();
		if (s) {
			std::copy(s->tokens().begin(), s->tokens().end(),
				std::back_inserter(token_buf_));
		} else {
			more = false;
		}
	}
	if (token_buf_.empty()) {
		return NULL;
	} else {
		Token* t = token_buf_.front();
		token_buf_.pop_front();
		return t;
	}
}

Sentence::Ptr BufferedSentenceReader::get_next_sentence()
{
	if (sentence_buf_ != NULL) {
		Sentence::Ptr s = sentence_buf_;
		sentence_buf_.reset();
		return s;
	} else {
		return actual_next_sentence();
	}
}

boost::shared_ptr<Chunk> BufferedSentenceReader::get_next_chunk()
{
	Sentence::Ptr s = get_next_sentence();
	if (!s) {
		return boost::shared_ptr<Chunk>();
	} else {
		boost::shared_ptr<Chunk> c = boost::make_shared<Chunk>();
		c->append(s);
		s = get_next_sentence();
		while (s && (!chunkify_ || s->first_token()->wa() !=
				PwrNlp::Whitespace::ManyNewlines)) {
			c->append(s);
			s = get_next_sentence();
		}
		if (s) {
			sentence_buf_ = s;
		}
		return c;
	}
}

} /* end ns Corpus2 */
