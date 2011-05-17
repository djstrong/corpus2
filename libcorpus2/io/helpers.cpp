#include <libcorpus2/io/helpers.h>
#include <libcorpus2/io/reader.h>
#include <sstream>
namespace Corpus2 {

std::vector<boost::shared_ptr<Chunk> > read_chunks_from_utf8_string(
        const std::string& data,
        const Tagset& tagset,
        const std::string& format)
{
	std::stringstream ss;
	ss << data;
	boost::shared_ptr<TokenReader> reader = TokenReader::create_stream_reader(
		format, tagset, ss);
	std::vector<boost::shared_ptr<Chunk> > chunks;
	while (boost::shared_ptr<Chunk> c = reader->get_next_chunk()) {
		chunks.push_back(c);
	}
	return chunks;
}

} /* end ns Corpus2 */
