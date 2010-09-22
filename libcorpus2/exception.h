#ifndef LIBCORPUS2_EXCEPTION_H
#define LIBCORPUS2_EXCEPTION_H

#include <libpwrutils/exception.h>

namespace Corpus2 {

/**
 * Base class for all Corpus2 errors. Derives from
 * @c PwrNlp::Error. Call member function @c what to get a
 * human-readable message associated with the error.
 */
class Corpus2Error : public PwrNlp::PwrNlpError
{
public:
	/**
	 * Instantiate an Error instance with the given message.
	 * @param what The message to associate with this error.
	 */
	Corpus2Error(const std::string &what);

	~Corpus2Error() throw();

	/// PwrNlpError override
	std::string scope() const;
};

class FileNotFound : public Corpus2Error
{
public:
	FileNotFound(const std::string& filename, const std::string& paths,
			const std::string& where);

	~FileNotFound() throw();

	std::string info() const;

	std::string filename, paths, where;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_EXCEPTION_H
