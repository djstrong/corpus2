#ifndef LIBCORPUS2_WHOLE_DOCUMENTCORPUSREADER_H
#define LIBCORPUS2_WHOLE_DOCUMENTCORPUSREADER_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <libcorpus2_whole/corpus.h>
#include <libcorpus2_whole/io/reader_i.h>

namespace Corpus2 {
namespace whole {

/**
 * Reads "document-like cropus"
 * Reads all documents in given file and returns their Corpus representation
 */
class DocumentCorpusReader : public CorpusReaderI
{
public:
	DocumentCorpusReader(const Tagset& tagset);

	/**
	 * Reads corpus from given path
	 * @arg corpus_file Path to file contains paths to corpus files.
	 *      Each lines in given corpus file contains two paths, to relations
	 *      and annotatons (in one line, first is path to annotations
	 *      and second are relations -- these paths, should be separated
	 *      by semicolon)
	 * @return Readed corpus
	 */
	boost::shared_ptr<Corpus> read(const std::string& corpus_file);

	/**
	 * Sets options for readers (relation reader and/or ccl reader).
	 * Available options:
	 *  - autogen_sent_id -- for automatically generation identifiers of sentences
	 */
	void set_option(const std::string& option);

private:
	const Tagset& tagset_;
};

} // whole ns
} // Corpus2 ns

#endif // LIBCORPUS2_WHOLE_DOCUMENTCORPUSREADER_H
