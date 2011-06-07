#ifndef LIBSORPUS2_PQREADER_H
#define LIBCORPUS2_PQREADER_H

#include <libcorpus2/io/reader.h>
#include <deque>
#include <boost/scoped_ptr.hpp>

/*
extern "C" {
void pwrnlp_plugin_init();
}
*/

namespace Corpus2 {

// fwd decl
class PoliqarpClient;

class PoliqarpReader: public TokenReader
{
public:
	PoliqarpReader(const Tagset& tagset, const std::string& filename);

	enum PQ_MODE {
		PQ_TOKENS,
		PQ_SENTENCES,
		PQ_CHUNKS,
		PQ_MANUAL
	};

	~PoliqarpReader();

	void set_query(const std::string& query);

	void execute();

	Token* get_next_token();

	Sentence::Ptr get_next_sentence();

	boost::shared_ptr<Chunk> get_next_chunk();

	void set_option(const std::string& option);

	std::string get_option(const std::string& option) const;

	static bool registered;

protected:
	void ensure_more();

	boost::scoped_ptr<PoliqarpClient> pq_;

	bool executed_;

	PQ_MODE mode_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_PQREADER_H
