#ifndef LIBSORPUS2_IO_HELPERS_H
#define LIBCORPUS2_IO_HELPERS_H

#include <libcorpus2/chunk.h>
#include <libcorpus2/tagset.h>

namespace Corpus2 {

std::vector<boost::shared_ptr<Chunk> > read_chunks_from_utf8_string(
	const std::string& data,
	const Tagset& tagset,
	const std::string& format);

} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_HELPERS_H
