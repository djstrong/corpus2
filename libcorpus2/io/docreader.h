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

#ifndef LIBCORPUS2_DOCREADER_H
#define LIBCORPUS2_DOCREADER_H

#include <libcorpus2/io/reader.h>

namespace Corpus2 {

/**
 * A reader for whole documents. Note that a whole document is read into memory
 * before any processing may take place.
 */
class DocumentReader {
public:
	/**
	 * Reads a whole document, using the two given path: the morphosyntax and
	 * chunk-style annotations are read from annot_path, while relations
	 * between chunk-style annotations are read from rela_path.
	 * Both path may in particular point to the same path.
	 * TODO!
	 */
	DocumentReader(const std::string &annot_path,
				   const std::string &rela_path,
				   const std::string &rdr_class_id = "ccl");
};
} /* end ns Corpus2 */

#endif // LIBCORPUS2_DOCREADER_H
