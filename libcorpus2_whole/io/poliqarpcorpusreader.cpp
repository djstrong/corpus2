#include <libcorpus2_whole/io/poliqarpcorpusreader.h>

namespace Corpus2 {
namespace whole{

PoliqarpCorpusReader::PoliqarpCorpusReader(const Tagset& tagset)
	: tagset_(tagset)
{
	//
}

boost::shared_ptr<Corpus> PoliqarpCorpusReader::read(const std::string& corpus_file)
{
	boost::shared_ptr<Document> doc;
	boost::shared_ptr<Corpus> corpus = boost::make_shared<Corpus>(corpus_file);

	this->pq_doc_reader_ = boost::shared_ptr<PoliqarpDocumentReader>(
			new PoliqarpDocumentReader(tagset_, corpus_file));

	while ((doc = this->pq_doc_reader_->read())) {
		corpus->add_document(doc);
	}

	return corpus;
}

} // whole ns
} // Corpus2 ns
