#include <libcorpus2/guesser/guesser.h>
#include <libcorpus2/tagsetmanager.h>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH


using namespace	Corpus2;


int main(int argc, const char ** argv)
{
	const Tagset & t = get_named_tagset("nkjp");
	
	Guesser g(argv[1], t);
	
	std::cout << "> ";
	std::cout.flush();
	std::string input;
	std::cin >> input;
	
	while (true)
	{
		std::vector<Lexeme> lexemes = g.guess(UnicodeString(input.c_str()).trim());
		foreach (Lexeme & l, lexemes)
		{
			std::cout << l.lemma() << " " << t.tag_to_string(l.tag()) << std::endl;
		}
		
		std::cout << "\n> ";
		std::cout.flush();
		std::cin >> input;
	}
}
