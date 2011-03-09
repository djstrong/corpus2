#ifndef LIBSORPUS2_IO_XMLWRITER_H
#define LIBCORPUS2_IO_XMLWRITER_H

#include <libcorpus2/io/writer.h>

namespace Corpus2 {

/**
 * Base class for xml-ish writers
 */
class XmlWriter : public TokenWriter {
public:
	XmlWriter(std::ostream& os, const Tagset& tagset,
			const string_range_vector& params);

	~XmlWriter();

	void write_token(const Token &t);

	void write_sentence(const Sentence &s);

protected:
	void do_header();

	void do_footer();

	virtual void paragraph_head();

	void paragraph_head(const Chunk& c);

	int cid_;

	bool use_indent_;

	bool output_disamb_;

	bool sort_tags_;

	bool whitespace_info_;
};


} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_XMLWRITER_H
