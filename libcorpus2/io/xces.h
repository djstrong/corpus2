#ifndef LIBCORPUS2_IO_XCES_H
#define LIBCORPUS2_IO_XCES_H

#include <libcorpus2/tagset.h>
#include <libcorpus2/tokensource.h>

namespace Corpus2 {

class XcesError : public Error
{
public:
	/// Constructor
	XcesError(const std::string &what);

	/// Destructor
	~XcesError() throw();
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_XCES_H
