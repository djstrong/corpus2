#include <libcorpus2_whole/io/poliqarpdocumentreader.h>
#include <boost/scoped_ptr.hpp>

namespace Corpus2 {
namespace whole {

PoliqarpDocumentReader::PoliqarpDocumentReader(const Tagset& tagset, const std::string& corpus_path, const std::string& corpus_reader)
	: DocumentReaderI("poliqarp")
{
	this->pqr_ = Corpus2::TokenReader::create_path_reader(corpus_reader, tagset, corpus_path);
}

PoliqarpDocumentReader::PoliqarpDocumentReader(const Tagset& tagset, const std::string& corpus_path)
	: DocumentReaderI("poliqarp")
{

	this->pqr_ = Corpus2::TokenReader::create_path_reader("poliqarp", tagset, corpus_path);
}

boost::shared_ptr<Document> PoliqarpDocumentReader::read()
{
	boost::shared_ptr<Document> document;
	boost::shared_ptr<Chunk> chunk = this->pqr_->get_next_chunk();

	if (chunk) {
		document = boost::make_shared<Document>();
		document->add_paragraph(chunk);
	}
	else
	{
		document = boost::make_shared<Document>("End");
	}
	return document;
}


} // whole ns
} // Corpus2 ns
