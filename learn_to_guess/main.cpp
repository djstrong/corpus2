#include "tree.h"
#include "config.h"

#include <fstream>
#include <set>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace Corpus2;




set<UnicodeString> prepare_morphfile(const path & morphfile)
{
	set<UnicodeString> result;
	
	ifstream file(morphfile.c_str());
	
	string line;
	while (getline(file, line))
	{
		std::vector <std::string> splt;
		split(splt, line, is_any_of("\t"));
		
		result.insert(UnicodeString(splt[0].c_str()));
	}
	
	return result;
}

void add_corpus(const path & path, const set<UnicodeString> & real_words, Tree & tree)
{	
	const Tagset & tagset = Cfg.getTagset();
	TokenReader::TokenReaderPtr reader = Cfg.getReader(path);
	Sentence::Ptr sentence;
	while((sentence = reader->get_next_sentence()))
	{
		foreach (const Token * token, sentence->tokens())
		{
			if (real_words.find(token->orth()) != real_words.end())
			{
				const Lexeme & lexeme = token->get_preferred_lexeme(tagset);
				tree.insert(token->orth(), lexeme.lemma(), lexeme.tag());
			}
		}
	}
}


int main(int argc, const char ** argv)
{
	try {
		config::create(argc, argv);
		
		
		Tree tree;
		
		
		/*
		tree.insert("hamak", "aaa", Cfg.getTagset().make_ign_tag());
		tree.insert("hamak", "aaa", Cfg.getTagset().make_ign_tag());
		tree.insert("aaaaa", "aaa", Cfg.getTagset().make_ign_tag());
		tree.insert("aaaa", "aaa", Cfg.getTagset().make_ign_tag());
		tree.insert("kalafior", "aaa", Cfg.getTagset().make_ign_tag());
		tree.insert("kaladior", "aaa", Cfg.getTagset().make_ign_tag());
		/*/
		if (Cfg.isVerbose()) cout << "Preparing morphfile " << Cfg.getMorphfile() << endl;
		set<UnicodeString> real_words = prepare_morphfile(Cfg.getMorphfile());
		
		if (Cfg.isVerbose()) cout << "Reading corpora" << endl;
		foreach (path corpus, Cfg.getCorpora())
		{
			if (Cfg.isVerbose()) cout << "\treading through " << corpus << endl;
			add_corpus(corpus, real_words, tree);
		}
		//*/
		
		tree.compress();
		tree.prune();
		
		tree.write(Cfg.getOutfile());
		
		/*
		if (Cfg.isVerbose()) cout << "Compressing and pruning" << endl;
		tree.compress();
		tree.prune();
		
		if (Cfg.isVerbose()) cout << "Dumping" << endl;
		
		ofstream file(Cfg.getOutfile().c_str(), ofstream::trunc | ofstream::binary);
		//tree.dump(file);
		
		if (Cfg.isVerbose()) cout << "Done." << endl;
		*/
	}
	catch (const config::Skip & e)
	{
		// Do nothing.
	}
	catch (const program_options::error & e)
	{
		cerr << "Command line error: " << e.what() << endl;
	}
	catch (const std::exception & e)
	{
		cerr << "Error: " << e.what() << endl;
	}
	catch (...)
	{
		cerr << "Unknown exception!" << endl;
	}
}
