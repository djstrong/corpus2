#include <libcorpus2_whole/io/corpusreader.h>
#include <libcorpus2_whole/io/documentcorpusreader.h>

#ifdef WITH_POLIQARP
#include <libcorpus2_whole/io/poliqarpcorpusreader.h>
#endif


namespace Corpus2 {
namespace whole{

CorpusReader::CorpusReader(const Tagset& tagset, const std::string& corpus_type)
	: corpus_type_(corpus_type), tagset_(tagset)
{
	//
}

boost::shared_ptr<Corpus> CorpusReader::read(const std::string& corpus_file_path)
{
	try {
		boost::shared_ptr<CorpusReaderI> reader = this->get_corpus_reader_by_type();
		return reader->read(corpus_file_path);
	} catch(...) {
		throw;
	}
}

//
boost::shared_ptr<CorpusReaderI> CorpusReader::get_corpus_reader_by_type()
{
	if (corpus_type_ == "document") {
		return boost::shared_ptr<DocumentCorpusReader>(
				new DocumentCorpusReader(tagset_));
#ifdef WITH_POLIQARP
	} else if (corpus_type_ == "poliqarp") {
		return boost::shared_ptr<PoliqarpCorpusReader>(
				new PoliqarpCorpusReader(tagset_));
#endif
	}
	throw Corpus2Error(corpus_type_ + " is an unknown reader type!");
}

} // whole ns
} // Corpus2 ns
