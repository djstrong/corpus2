#include <libcorpus2_whole/io/poliqarpdocumentreader.h>

namespace Corpus2 {
namespace whole {

PoliqarpDocumentReader::PoliqarpDocumentReader(const Tagset& tagset, const std::string& corpus_path)
	: DocumentReaderI("poliqarp"), corpus_path_(corpus_path)
{
	this->pqr_ = boost::shared_ptr<PoliqarpReader> (new PoliqarpReader(tagset, corpus_path));
}

boost::shared_ptr<Document> PoliqarpDocumentReader::read()
{
	boost::shared_ptr<Document> document = boost::make_shared<Document>();
	// boost::shared_ptr<Chunk> chunk = this->pqr_->pq_->get_next_sentence();
//	if (chunk) {
//		document->add_paragraph(chunk);
//	}
	return document;
}

} // whole ns
} // Corpus2 ns
