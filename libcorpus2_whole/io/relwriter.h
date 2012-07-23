/*
        Copyright (C) 2010 Tomasz Åniatowski, Adam Radziszewski, PaweÅ KÄdzia
        Part of the libcorpus2 project

        This program is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.

        This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.

        See the LICENSE.CORPUS2, LICENSE.POLIQARP, COPYING.LESSER and COPYING files for more details.
*/


#ifndef LIBCORPUS2_WHOLE_RELWRITER_H
#define LIBCORPUS2_WHOLE_RELWRITER_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include <libcorpus2_whole/relation.h>

#include <libcorpus2/io/xmlwriter.h>

#include <iostream>

namespace Corpus2 {
namespace whole {

	
/**
 * A reader for realtion documents. Note that document is read into memory
 * before any processing may take place.
 */
class RelationWriter {
public:
	/**
	 * Reads a document with relations
	 * @param rela_path  path to file with relations
	 */
	RelationWriter(const std::string &rela_path);

	/**
	 * Lazy relations accessor.
	 * If relations are not readed then read relations and returns list of them.
	 * @return List of readed relations
	 */
	void write(const std::vector< boost::shared_ptr<Relation> >& relations);


private:

  void write_relation(const boost::shared_ptr<Relation>& r);
  
  void do_header();

	void do_footer();
	

	// -------------------------------------------------------------------------
	/// List of the relations in given relation file
	std::vector< boost::shared_ptr<Relation> > relations_;

	/// Path to file with relations
	std::string rela_path_;

	/// File pointer
	boost::shared_ptr<std::ofstream> file_;

	// -------------------------------------------------------------------------
	// Temporary information of actual parsing relation
	std::string rel_name_;
	std::string ann_number_;
	boost::shared_ptr<DirectionPoint> rel_from_;
	boost::shared_ptr<DirectionPoint> rel_to_;
};

} // whole ns
} // Corpus2 ns

#endif // LIBCORPUS2_WHOLE_RELREADER_H

