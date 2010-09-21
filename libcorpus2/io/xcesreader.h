#ifndef LIBCORPUS2_IO_XCESREADER_H
#define LIBCORPUS2_IO_XCESREADER_H

#include <libcorpus2/io/reader.h>
#include <libcorpus2/io/xces.h>
#include <libcorpus2/chunk.h>
#include <deque>
#include <boost/scoped_ptr.hpp>

namespace Corpus2 {

class XcesReaderImpl;

class XcesReader : public BufferedChunkReader
{
public:
	XcesReader(const Tagset& tagset, std::istream& is,
			bool disamb_only = false);

	~XcesReader();

	std::istream& is() {
		return is_;
	}

protected:
	void ensure_more();

	std::istream& is_;

	boost::scoped_ptr<XcesReaderImpl> impl_;
};


} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_XCESREADER_H
