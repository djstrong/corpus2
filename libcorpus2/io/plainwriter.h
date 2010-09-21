#ifndef LIBCORPUS2_IO_PLAIN_H
#define LIBCORPUS2_IO_PLAIN_H

#include <libcorpus2/io/writer.h>

namespace Corpus2 {

class PlainWriter : public TokenWriter
{
public:
	PlainWriter(std::ostream& os, const Tagset& tagset,
			const string_range_vector& params);

	void write_token(const Token& t);

	void write_sentence(const Sentence& t);

	void write_chunk(const Chunk& c);

	static bool registered;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_PLAIN_H
