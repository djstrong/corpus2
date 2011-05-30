#ifndef CORPUS2_POLIQARP_PQCLIENT_H
#define CORPUS2_POLIQARP_PQCLIENT_H

#include <boost/utility.hpp>

namespace Corpus2
{

class PoliqarpClient : boost::noncopyable
{
public:
	PoliqarpClient(const std::string path);
	~PoliqarpClient();

	void reload_corpus(const std::string& path);
	void restart();
	int execute_query();
	int compile_query(const std::string& q);

	size_t get_corpus_size() const;
private:
};

}

#endif /* CORPUS2_POLIQARP_PQCLIENT_H */
