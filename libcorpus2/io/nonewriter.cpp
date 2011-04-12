#include <libcorpus2/io/nonewriter.h>

namespace Corpus2 {

bool NoneWriter::registered = TokenWriter::register_writer<NoneWriter>(
	"none");

NoneWriter::NoneWriter(std::ostream& os, const Tagset& tagset,
		const string_range_vector& params)
	: TokenWriter(os, tagset, params)
{
}

void NoneWriter::write_token(const Token&)
{
}

void NoneWriter::write_sentence(const Sentence&)
{
}

void NoneWriter::write_chunk(const Chunk&)
{
}


} /* end ns Corpus2 */
