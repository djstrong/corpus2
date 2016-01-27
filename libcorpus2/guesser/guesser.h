#ifndef LIBCORPUS2_GUESSER_GUESSER_H
#define LIBCORPUS2_GUESSER_GUESSER_H

#include <libcorpus2/guesser/backwardtrie.h>
#include <libcorpus2/lexeme.h>


namespace Corpus2 {

namespace detail
{
	typedef std::pair<UnicodeString, int> recipe;
	struct tags_info : public std::map <Corpus2::Tag, recipe>
	{
		void print(std::ostream& stream) const;
		void dedump(std::istream& stream);
	};
}

class Guesser
{
	const Tagset & tagset;
	
	struct Tree : public BackwardTrie<detail::tags_info>
	{
		const detail::tags_info & get(const UnicodeString &word) const;
	} tree;
	
	
public:
	/// Reads learned data from given file.
	Guesser(const boost::filesystem::path & path, const Tagset &tagset);
	
	/// Returns possible interpretations of given word.
	std::vector<Lexeme> guess(const UnicodeString & word) const;
	
	/// Debug printing, do not use.
	void print() const;
};


} /* end ns Corpus2 */

std::ostream & operator<< (std::ostream& stream, const Corpus2::detail::tags_info & props);
std::istream & operator>> (std::istream& stream, Corpus2::detail::tags_info & props);

#endif // LIBCORPUS2_GUESSER_GUESSER_H
