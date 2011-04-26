#include <libcorpus2/io/pathwriter.h>

namespace Corpus2 {

PathWriter::PathWriter(const boost::shared_ptr<TokenWriter>& underlying,
		const boost::shared_ptr<std::ostream>& os)
	: TokenWriter(*os, underlying->tagset(), string_range_vector()),
	 os_(os), underlying_(underlying)
{
	assert(&underlying_->os() == os_.get());
}

PathWriter::~PathWriter()
{
}

void PathWriter::write_token(const Token& t)
{
	underlying_->write_token(t);
}

void PathWriter::write_sentence(const Sentence& s)
{
	underlying_->write_sentence(s);
}

void PathWriter::write_chunk(const Chunk& p)
{
	underlying_->write_chunk(p);
}

} /* end ns Corpus2 */
