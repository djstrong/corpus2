/*
	Copyright (C) 2010 Tomasz Śniatowski, Adam Radziszewski, Paweł Kędzia
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

#include <boost/make_shared.hpp>
#include <libcorpus2_whole/io/cclrelreader.h>

namespace Corpus2 {
namespace whole {
	CclRelReader::CclRelReader(const Tagset& tagset,
		const std::string &annot_path, const std::string &rela_path)
			: DocumentReaderI("document")
	{
		make_readers(tagset, annot_path, rela_path);
		make_id_doc(annot_path, rela_path);
	}

	void CclRelReader::make_readers(const Tagset& tagset,
		const std::string &annot_path, const std::string &rela_path)
	{
		ccl_reader_ = boost::make_shared<CclReader>(tagset, annot_path);
		// prevent the underlying CCL reader from complaining about
		// relation XML tags unknown to the reader itself
		// (in case annot_path and rela_path poin to the same file)
		ccl_reader_->set_option("no_warn_unexpected_xml");
		rel_reader_ = boost::make_shared<RelationReader>(rela_path);
	}

	void CclRelReader::make_id_doc(const std::string &annot_path,
		const std::string &rela_path)
	{
		id_ = (annot_path + ";" + rela_path);
	}

	boost::shared_ptr<Document> CclRelReader::read()
	{
		boost::shared_ptr<Chunk> chunk;
		boost::shared_ptr<Document> document = boost::make_shared<Document>(id_);

		// Read ccl document and makes document
		while (1) {
			chunk = ccl_reader_->get_next_chunk();
			if (!chunk) {
				break;
			}
			else {
				document->add_paragraph(chunk);
			}
		}

		// Read relations and adds them to the document
		const std::vector< boost::shared_ptr<Relation> > relations =
				rel_reader_->relations();
		for (unsigned int i = 0; i < relations.size(); i++) {
			document->add_relation(relations[i]);
		}

		return document;
	}

	void CclRelReader::set_option(const std::string& option)
	{
		if (option == "autogen_sent_id") {
			ccl_reader_->set_option("autogen_sent_id");
		} else if (option == "autogen_chunk_id") {
			ccl_reader_->set_option("autogen_chunk_id");
		}
	}

	std::string CclRelReader::get_option(const std::string& option) const {
		if (option == "autogen_sent_id") {
			return ccl_reader_->get_option("autogen_sent_id");
		}
		else if (option == "autogen_chunk_id") {
			return ccl_reader_->get_option("autogen_chunk_id");
		}
		return "";
	}

} // whole ns
} // Corpus2 ns
