#ifndef LIBCORPUS2_TAGSETPARSER_H
#define LIBCORPUS2_TAGSETPARSER_H

#include <libcorpus2/tagset.h>

namespace Corpus2 {

class TagsetParseError : public Corpus2Error
{
public:
	TagsetParseError(const std::string& w, int line,
			const std::string& data)
		: Corpus2Error("Tagset parse error: " + w), line(line), data(data)
	{
	}

	~TagsetParseError() throw()
	{
	}

	virtual std::string info() const;

	int line;
	std::string data;
};

class TagsetParser
{
public:
	static Tagset load_ini(const std::string& filename);

	static Tagset load_ini(std::istream& is);

	static void save_ini(const Tagset& tagset,
			const std::string& filename);

	static void save_ini(const Tagset& tagset, std::ostream& os);

};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_TAGSETPARSER_H
