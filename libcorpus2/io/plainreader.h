#ifndef LIBSORPUS2_IO_PLAINREADER_H
#define LIBCORPUS2_IO_PLAINREADER_H

#include <libcorpus2/io/reader.h>
#include <boost/scoped_ptr.hpp>

namespace Corpus2 {

class PlainReader : public BufferedSentenceReader
{
public:
	PlainReader(const Tagset& tagset, std::istream& is);

	PlainReader(const Tagset& tagset, const std::string& filename);

	std::istream& is() {
		return *is_;
	}

	void set_option(const std::string& option);

	std::string get_option(const std::string& option) const;

	static bool registered;

protected:
	/// BufferedSentenceReader override
	Sentence::Ptr actual_next_sentence();

	std::istream* is_;
	boost::scoped_ptr<std::istream> is_owned_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_PLAINREADER_H
