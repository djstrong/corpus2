
#include <boost/algorithm/string.hpp>
#include <libcorpus2_whole/io/documentreader.h>
#include <libcorpus2_whole/io/cclrelreader.h>


#include <libcorpus2_whole/io/poliqarpdocumentreader.h>



namespace Corpus2 {
namespace whole{

DocumentReader::DocumentReader(const Tagset& tagset, const std::string& corpus_type, const std::string& corpus_file_path)
	: corpus_type_(corpus_type), tagset_(tagset), corpus_path_(corpus_file_path)
{
	if (corpus_type_ == "poliqarp")
	{
		reader = boost::shared_ptr<PoliqarpDocumentReader>(
						new PoliqarpDocumentReader(tagset_, corpus_path_));
	}
	else if (corpus_type_ == "document")
        {
            corpus_file.open(corpus_file_path.c_str());
	}
	else
		throw Corpus2Error(corpus_type_ + " is an unknown reader type!");

}

boost::shared_ptr<Document> DocumentReader::read()
{

	std::string line;
	if (corpus_type_ == "poliqarp")
	{
		return this->reader->read();
	}
        if (corpus_type_ == "document")
	{
            if (std::getline(corpus_file, line))
		{
                        return get_cclrel_reader(line)->read();
                        //return Document("End");
		}
		else
		{
                    return boost::make_shared<Document>("End");
		}
	}
}


boost::shared_ptr<DocumentReaderI> DocumentReader::get_cclrel_reader(std::string& line)
{
	std::string ann_path, rel_path;

	// split line by semicolon
	std::vector<std::string> splitted_line;
	boost::split(splitted_line, line, boost::is_any_of(";"));

	if (splitted_line.empty()) {
                throw Corpus2Error("Empty line in corpus file!");
	}
	else if (splitted_line.size() == 1) {
		throw Corpus2Error("CclRelReader requires both paths to relations and annotations");
	}

	ann_path = splitted_line[0];
	rel_path = splitted_line[1];

	return boost::shared_ptr<CclRelReader>(
			new CclRelReader(tagset_, ann_path, rel_path));



}


} // whole ns
} // Corpus2 ns
