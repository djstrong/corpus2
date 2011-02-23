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

#ifndef LIBCORPUS2_SENTENCE_H
#define LIBCORPUS2_SENTENCE_H

#include <libcorpus2/token.h>
#include <libpwrutils/sentence.h>
#include <boost/shared_ptr.hpp>

namespace Corpus2 {

class Sentence : private boost::noncopyable
{
public:
	typedef boost::shared_ptr<Sentence> Ptr;
	typedef boost::shared_ptr<const Sentence> ConstPtr;

	/// Empty constructor
	Sentence();

	virtual Ptr clone_shared() const;

	/// Destructor
	virtual ~Sentence();

	void release_tokens();

	bool empty() const {
		return tokens_.empty();
	}

	/// Size accessor
	size_t size() const {
		return tokens_.size();
	}

	/// Token accessor
	Token* operator[](size_t idx) {
		return tokens_[idx];
	}

	/// Token accessor, const
	const Token* operator[](size_t idx) const {
		return tokens_[idx];
	}

	/// Underlying vector accessor, const
	const std::vector<Token*>& tokens() const {
		return tokens_;
	}

	/// Underlying vector accessor
	std::vector<Token*>& tokens() {
		return tokens_;
	}

	/// Helper function for appending tokens
	/// Might be overriden in a child class to make adding a token keep
	/// extra invariants
	virtual void append(Token* t) {
		tokens_.push_back(t);
	}

	/// convenience first token accessor
	const Token* first_token() const {
		assert(!tokens_.empty());
		return tokens_[0];
	}

protected:
	/// The tokens this sentence contains and owns
	std::vector<Token*> tokens_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_SENTENCE_H
