#include <fstream>
#include <boost/algorithm/string.hpp>

#include <libcorpus2/exception.h>
#include <libcorpus2_whole/corpusreader.h>

namespace Corpus2 {
namespace whole {

CorpusReader::CorpusReader(const Tagset& tagset, const std::string& corpus_type)
	: tagset_(tagset), corpus_type_(corpus_type)
{
	//
}

boost::shared_ptr<Corpus> CorpusReader::read(const std::string& corpus_file_path)
{
	std::string line;
	std::ifstream corpus_file(corpus_file_path.c_str());
	if (!corpus_file) {
		throw Corpus2Error(corpus_file_path + " file not found!");
	}

	boost::shared_ptr<Corpus> corpus = boost::make_shared<Corpus>(corpus_file_path);

	while(getline(corpus_file, line)) {
		boost::shared_ptr<DocumentReaderI> reader;
		std::string ann_path, rel_path;
		// split line by semicolon
		std::vector<std::string> splitted_line;
		boost::split(splitted_line, line, boost::is_any_of(";"));
		if (splitted_line.empty()) {
			continue;
		}
		else if (splitted_line.size() == 1) {
			ann_path = splitted_line[0];
			rel_path = "";
		}
		else {
			ann_path = splitted_line[0];
			rel_path = splitted_line[1];
		}
		reader = this->get_reader_by_type(this->corpus_type_, ann_path, rel_path);
		boost::shared_ptr<Document> document = reader->read();
		corpus->add_document(document);
	}

	return corpus;
}

//
boost::shared_ptr<DocumentReaderI> CorpusReader::get_reader_by_type(
	const std::string &type,
	const std::string &ann_path,
	const std::string &rel_path)
{
	/*if (type == "poliqarp") {
		static boost::shared_ptr<PoliqarpDocumentReader> pq_doc_reader;
		if (!pq_doc_reader) {
			pq_doc_reader = boost::shared_ptr<PoliqarpDocumentReader>(
					new PoliqarpDocumentReader(this->tagset_, ann_path));
		}
		return pq_doc_reader;
	} else */if (type == "document") {
		return boost::shared_ptr<DocumentReader>(
				new DocumentReader(this->tagset_, ann_path, rel_path));
	}
	throw Corpus2Error(type + " is unknown reader type!");
}

} // whole ns
} // Corpus2 ns
