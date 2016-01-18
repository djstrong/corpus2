#ifndef LIBCORPUS2_GUESSER_BACKWARDTRIE_H
#define LIBCORPUS2_GUESSER_BACKWARDTRIE_H

#include <unicode/unistr.h>

#include <map>
#include <boost/function.hpp>
#include <boost/filesystem.hpp>

#include <libcorpus2/tag.h>



namespace Corpus2 {


/**
  Trie that stores all strings backward
  */
template <typename PropType>
class BackwardTrie
{
protected:
	/// Internal node of the tree.
	struct Node
	{
		Node *first_child, *next_sibling;
		UChar character;
		
		PropType properties;
		
		
		Node(UChar character = '\0');
		virtual ~Node();
		
		void write(std::ostream & stream) const;
		void read(std::istream & stream);
		void print(int d = 0) const;
		
		
		/// Returns the last child.
		Node * lastChild() const;
		
		/// Counts children and returns their number.
		int childrenCount() const;
	};
	
	Node root;
	
	
	/// Finds node for given word, going as far as it can, but not further.
	/// Calls f on each node it passes.
	Node & find(const UnicodeString & word, boost::function<void(PropType &)> f = NULL);
	
	/// Finds node for given word, going as far as it can, but not further.
	/// Calls f on each node it passes. Const version.
	const Node & find(const UnicodeString & word, boost::function<void(const PropType &)> f = NULL) const;
	
public:
	BackwardTrie();
	
	/// Writes the tree into a file. Requires that the PropType be compatible
	/// with output streams, i.e. have operator<<(ostream, PropType).
	void write(const boost::filesystem::path & path) const;
	
	/// Reads the tree from given file. Requires that the PropType be compatible
	/// with input streams, i.e. have operator>>(ostream, PropType).
	void read(const boost::filesystem::path & path);
	
	/// Debug print. Do not use.
	void print() const;
	
private:
	Node & find_aux(Node & node, const UnicodeString &word, boost::function<void(PropType &)> f, int i);
	const Node & find_aux(const Node & node, const UnicodeString &word, boost::function<void(const PropType &)> f, int i) const;
	
};

} /* end ns Corpus2 */

#include <libcorpus2/guesser/backwardtrie.tpp>

#endif // LIBCORPUS2_GUESSER_BACKWARDTRIE_H
