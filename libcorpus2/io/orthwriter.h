#ifndef LIBCORPUS2_IO_ORTHWRITER_H
#define LIBCORPUS2_IO_ORTHWRITER_H

#include <libcorpus2/io/writer.h>

namespace Corpus2 {

class OrthWriter : public TokenWriter {
public:
	OrthWriter(std::ostream& os, const Tagset& tagset,
			const string_range_vector& params);

	~OrthWriter();

	void write_token(const Token &t);

	void write_sentence(const Sentence &s);

	void write_chunk(const Chunk &c);

	bool actual_ws_;

	bool end_nl_;

	static bool registered;

};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_ORTHWRITER_H
