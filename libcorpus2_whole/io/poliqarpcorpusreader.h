#ifndef LIBCORPUS2_WHOLE_POLIQARPCORPUSREADER_H
#define LIBCORPUS2_WHOLE_POLIQARPCORPUSREADER_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <libcorpus2_whole/corpus.h>
#include <libcorpus2_whole/io/poliqarpdocumentreader.h>
#include <libcorpus2_whole/io/reader_i.h>

namespace Corpus2 {
namespace whole {

class PoliqarpCorpusReader : public CorpusReaderI
{
public:
	PoliqarpCorpusReader(const Tagset& tagset);

	boost::shared_ptr<Corpus> read(const std::string& corpus_file);

private:
	const Tagset& tagset_;
	boost::shared_ptr<PoliqarpDocumentReader> pq_doc_reader_;
};

} // whole ns
} // Corpus2 ns

#endif // LIBCORPUS2_WHOLE_POLIQARPCORPUSREADER_H
