#ifndef LIBSORPUS2_IO_CCLWRITER_H
#define LIBCORPUS2_IO_CCLWRITER_H

#include <libcorpus2/io/xmlwriter.h>

namespace Corpus2 {

class CclWriter : public XmlWriter
{
public:
	CclWriter(std::ostream& os, const Tagset& tagset,
			const string_range_vector& params);

	~CclWriter();

	void write_sentence(const Sentence &s);

	void write_chunk(const Chunk &c);

	static bool registered;

protected:
	void do_header();

	void do_footer();

	void paragraph_head();

	void paragraph_head(const Chunk& c);
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_CCLWRITER_H
