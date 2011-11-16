/*
	Copyright (C) 2010 Tomasz Śniatowski, Adam Radziszewski, Paweł Kędzia
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

#ifndef LIBCORPUS2_RELREADER_H
#define LIBCORPUS2_RELREADER_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <libcorpus2/relation.h>

namespace Corpus2 {

/**
 * A reader for realtion documents. Note that document is read into memory
 * before any processing may take place.
 */
class RelationReader {
public:
	/**
	 * Reads a document with relations
	 * @param rela_path  path to file with relations
	 * TODO! Not implemented yet!
	 */
	RelationReader(const std::string &rela_path);

	/**
	 * Relations accessor. If relations are not readed then read relations
	 * and returns list of them.
	 * @return List of readed relations
	 */
	const std::vector< boost::shared_ptr<Relation> >& relations() {
		if (!readed_) {
			read();
		}

		return relations_;
	}

private:
	void read();

	/// List of the relations in given relation file
	std::vector< boost::shared_ptr<Relation> > relations_;

	/// Path to file with relations
	const std::string rela_path_;

	///
	bool readed_;
};
} /* end ns Corpus2 */

#endif // LIBCORPUS2_RELREADER_H
