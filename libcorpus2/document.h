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

#ifndef LIBCORPUS2_DOCUMENT_H
#define LIBCORPUS2_DOCUMENT_H

#include <libcorpus2/chunk.h>
#include <boost/shared_ptr.hpp>

namespace Corpus2 {

/**
 * A whole document, consisting of consecutive paragraphs ("chunks"), being
 * sequences of sentences.
 * Usage of this class assumes that a whole document is read into memory before
 * any further takes place.
 */
class Document
{
public:
	Document();
	~Document();

	void add_paragraph(const boost::shared_ptr<Chunk> para) {
		paragraphs_.push_back(para);
	}

	const std::vector< boost::shared_ptr<Chunk> >& paragraphs() const {
		return paragraphs_;
	}

protected:
	std::vector< boost::shared_ptr<Chunk> > paragraphs_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_DOCUMENT_H
