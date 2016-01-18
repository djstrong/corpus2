#include "config.h"


#include <libcorpus2/tagsetmanager.h>
#include <iostream>
#include <fstream>


using namespace Corpus2;
using namespace boost::filesystem;


config * config::instance = NULL;



config::config(int argc, const char ** argv)
{
	using namespace boost::program_options;
	
	path corpus;
	bool batch_mode;
	std::string tagset_name;
	
	
	options_description desc("Allowed options");
	desc.add_options()
			("help", "produce help message")
			("morphfile,m", value<path>(&morphfile)->required(), "morpheus dictionary through which all words shall be filtred")
			("corpus,c", value<path>(&corpus)->required(), "corpus whence statistcs will be gotten")
	        ("outfile,o", value<path>(&outfile)->required(), "file whither result should be stored")
	        ("batch,b", bool_switch(&batch_mode)->default_value(false), "if set, corpus will be treated as a list corporum")
	        ("tagset,t", value<std::string>(&tagset_name)->default_value("nkjp"), "tagset corporis")
	        ("reader,r", value<std::string>(&reader_name)->default_value("xces"), "reader for corpus")
	        ("verbose,v", bool_switch(&verbose)->default_value(false), "if the program should talk")
	;
	
	
	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	
	if (vm.count("help"))
		std::cout << desc << std::endl, throw Skip();
	
	notify(vm);
	
	
	if (batch_mode)
	{
		std::ifstream file(corpus.c_str());
		
		std::string line;
		while (getline(file, line))
			corpora.push_back(line);
	}
	else
		corpora.push_back(corpus);
	
	tagset = &Corpus2::get_named_tagset(tagset_name);
}
