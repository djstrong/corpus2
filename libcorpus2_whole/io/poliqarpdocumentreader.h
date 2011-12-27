#ifndef LIBCORPUS2_WHOLE_POLIQARPDOCUMENTREADER_H
#define LIBCORPUS2_WHOLE_POLIQARPDOCUMENTREADER_H

#include <poliqarp/pqreader.h>
#include <libcorpus2_whole/io/docreaderi.h>
#include <libcorpus2_whole/document.h>

namespace Corpus2 {
namespace whole {

class PoliqarpDocumentReader : public DocumentReaderI
{
public:
	PoliqarpDocumentReader(const Tagset& tagset, const std::string& corpus_path);

	/// semantic of this methd is like get_next_document
	boost::shared_ptr<Document> read();

private:
	const std::string corpus_path_;
	boost::shared_ptr<PoliqarpReader> pqr_;
};

} // whole ns
} // Corpus2 ns

#endif // LIBCORPUS2_WHOLE_POLIQARPDOCUMENTREADER_H
