#ifndef LIBCORPUS2_IO_XCESVALIDATE_H
#define LIBCORPUS2_IO_XCESVALIDATE_H

#include <boost/scoped_ptr.hpp>
#include <libcorpus2/tagset.h>
#include <libcorpus2/tokensource.h>

namespace Corpus2 {

class XcesValidatorImpl;

class XcesValidator
{
public:
	XcesValidator(const Tagset& tagset, std::ostream& out);

	~XcesValidator();

	void validate_stream(std::istream& is);

	void validate_file(const std::string& filename);

protected:
	boost::scoped_ptr<XcesValidatorImpl> impl_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_XCESVALIDATE_H
