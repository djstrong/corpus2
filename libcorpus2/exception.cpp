#include <libcorpus2/exception.h>
#include <sstream>
#include <libcorpus2/util/settings.h>
namespace Corpus2 {

	Error::Error(const std::string &what)
		: PwrNlp::Error(what)
	{
	}

	Error::~Error() throw()
	{
	}

	FileNotFound::FileNotFound(const std::string& filename,
			const std::string& paths, const std::string& where)
		: Error("File not found: " + filename), filename(filename),
		paths(paths), where(where)
	{
	}

	FileNotFound::~FileNotFound() throw()
	{
	}

	std::string FileNotFound::info() const
	{
		std::ostringstream ss;
		if (where.empty()) {
			ss << "File ";
		} else {
			ss << where << " file ";
		}
		ss << "'" << filename << "' not found in search path " << paths;
		return ss.str();
	}

} /* end ns Corpus2 */
