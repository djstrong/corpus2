#include <libcorpus2_whole/io/poliqarpdocumentreader.h>
#include <boost/scoped_ptr.hpp>

namespace Corpus2 {
namespace whole {

PoliqarpDocumentReader::PoliqarpDocumentReader(const Tagset& tagset, const std::string& corpus_path)
	: DocumentReaderI("poliqarp")
{
	this->pqr_ = boost::shared_ptr<PoliqarpReader>(new PoliqarpReader(tagset, corpus_path));
}

boost::shared_ptr<Document> PoliqarpDocumentReader::read()
{
	boost::shared_ptr<Document> document;
	boost::shared_ptr<Chunk> chunk = this->pqr_->get_next_chunk();

	if (chunk) {
		document = boost::make_shared<Document>();
		document->add_paragraph(chunk);
	}
	return document;
}

} // whole ns
} // Corpus2 ns
