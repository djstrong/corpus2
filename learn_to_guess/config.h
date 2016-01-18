#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <libcorpus2/tagset.h>
#include <libcorpus2/tagsetmanager.h>
#include <libcorpus2/io/reader.h>

#include <vector>


class config
{
public:
	
	class Skip {};  /// Exception teling to skip the rest of the prograp because there is nothing to do, eg. parameter --help were given.
	
	
	static config & get()
	{
		return *instance;
	}
	
	static config & create(int argc, const char ** argv)
	{
		delete instance;
		return *(instance = new config(argc, argv));
	}
	
	
	
	const Corpus2::Tagset & getTagset() const
	{
		return *tagset;
	}
	
	const Corpus2::TokenReader::TokenReaderPtr getReader(boost::filesystem::path corpus_path) const
	{
		return Corpus2::TokenReader::create_path_reader(reader_name, getTagset(), corpus_path.string());
	}
	
	const boost::filesystem::path & getMorphfile() const
	{
		return morphfile;
	}
	
	const std::vector<boost::filesystem::path> & getCorpora() const
	{
		return corpora;
	}
	
	const boost::filesystem::path & getOutfile() const
	{
		return outfile;
	}
	
	bool isVerbose() const
	{
		return verbose;
	}
	
	
	
private:
	
	static config * instance;
	
	boost::filesystem::path morphfile;
	std::vector<boost::filesystem::path> corpora;
	boost::filesystem::path outfile;
	const Corpus2::Tagset * tagset;
	std::string reader_name;
	bool verbose;
	
	
	
	config(int argc, const char ** argv);
	
};

#define Cfg config::get()


#endif // CONFIG_HPP
