#include <libcorpus2/exception.h>
#include <sstream>
#include <libcorpus2/util/settings.h>
namespace Corpus2 {

	Corpus2Error::Corpus2Error(const std::string &what)
		: PwrNlp::PwrNlpError(what)
	{
	}

	Corpus2Error::~Corpus2Error() throw()
	{
	}

	std::string Corpus2Error::scope() const
	{
		return "Corpus2";
	}

	FileNotFound::FileNotFound(const std::string& filename,
			const std::string& paths, const std::string& where)
		: Corpus2Error("File not found: " + filename), filename(filename),
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
