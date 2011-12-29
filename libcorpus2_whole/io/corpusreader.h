#ifndef LIBCORPUS2_WHOLE_CORPUSREADER_H
#define LIBCORPUS2_WHOLE_CORPUSREADER_H

#include <string>
#include <libcorpus2_whole/corpus.h>
#include <libcorpus2_whole/io/reader_i.h>

namespace Corpus2 {
namespace whole {

class CorpusReader
{
public:
	/**
	 * @arg corpus_type may be:
	 *  - document (contains relations)
	 *  - poliqarp
	 */
	CorpusReader(const Tagset& tagset, const std::string& corpus_type);

	/**
	 * Reads corpus from given path
	 * @arg corpus_file Path to file contains paths to corpus files.
	 *      Depend on corpus type, each line in this file should contains only
	 *      path to one document from corpus or path to (in particular DocReader)
	 *      relations and annotatons (in one line, first is path to annotations
	 *      and second are relations -- these paths, should be separated by semicolon)
	 * @return Readed corpus
	 */
	boost::shared_ptr<Corpus> read(const std::string& corpus_file);

private:
	/// Returns reader based on corpus type (poliqarp/document)
	boost::shared_ptr<CorpusReaderI> get_corpus_reader_by_type();

private:
	/// Type of corpus, sets only once in constructor
	const std::string& corpus_type_;

	/// Tagset to use, sets only onece in constructor
	const Tagset& tagset_;
};

} // whole ns
} // Corpus2 ns

#endif // LIBCORPUS2_WHOLE_CORPUSREADER_H
