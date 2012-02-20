#ifndef LIBCORPUS2_WHOLE_DOCUMENTREADER_H
#define LIBCORPUS2_WHOLE_DOCUMENTREADER_H
#include <fstream>
#include <string>
#include <libcorpus2_whole/corpus.h>
#include <libcorpus2_whole/io/reader_i.h>

namespace Corpus2 {
namespace whole {

class DocumentReader
{
public:
	DocumentReader(const Tagset& tagset, const std::string& corpus_type, const std::string& corpus_file_path, const std::string& corpus_reader);
	DocumentReader(const Tagset& tagset, const std::string& corpus_type, const std::string& corpus_file_path);
	boost::shared_ptr<Document> read();

private:

	boost::shared_ptr<DocumentReaderI> get_cclrel_reader(std::string& line);
	boost::shared_ptr<DocumentReaderI> reader;
        std::ifstream corpus_file;
private:
	const std::string corpus_type_;
	const Tagset& tagset_;
	const std::string corpus_path_;


};

} // whole ns
} // Corpus2 ns

#endif // LIBCORPUS2_WHOLE_DOCUMENTREADER_H
