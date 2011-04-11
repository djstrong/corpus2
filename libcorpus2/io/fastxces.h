#ifndef LIBSORPUS2_IO_FASTXCES_H
#define LIBCORPUS2_IO_FASTXCES_H

#include <libcorpus2/io/reader.h>
#include <libcorpus2/io/xces.h>
#include <libcorpus2/chunk.h>
#include <deque>
#include <boost/scoped_ptr.hpp>

namespace Corpus2 {

class FastXcesReaderImpl;

class FastXcesReader : public BufferedChunkReader
{
public:
	FastXcesReader(const Tagset& tagset, std::istream& is);

	FastXcesReader(const Tagset& tagset, const std::string& filename);

	~FastXcesReader();

	std::istream& is() {
		return *is_;
	}

	void set_option(const std::string& option);

	std::string get_option(const std::string& option);

	static bool registered;

protected:
	void ensure_more();

	// std::istream& is_;
	std::istream* is_;
	boost::scoped_ptr<std::istream> is_owned_;

	boost::scoped_ptr<FastXcesReaderImpl> impl_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_FASTXCES_H
