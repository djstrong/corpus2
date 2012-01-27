#ifndef CONLLWRITER_H
#define CONLLWRITER_H

#include <libcorpus2/io/writer.h>

namespace Corpus2 {

class ConllWriter : public TokenWriter
{
public:
	ConllWriter(std::ostream& os, const Tagset& tagset,
				const string_range_vector& params);

	~ConllWriter();

	void write_token(const Token &t);

	void write_sentence(const Sentence &s);

	void write_chunk(const Chunk &c);

	static bool registered;

protected:
	void do_header();

	void do_footer();
private:
	Tagset myTagset;
	std::string convert_tag(std::string tag);

};

} /* end ns Corpus2 */

#endif // CONLLWRITER_H
