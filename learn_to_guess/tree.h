#ifndef TREE_H
#define TREE_H


#include <map>

#include <boost/filesystem.hpp>

#include <libcorpus2/tag.h>
#include <libcorpus2/guesser/backwardtrie.h>



struct Properties
{
	int depth;  /// Distance from root.
	int counter;  /// How many words pass throught this letter.
	
	typedef std::pair<UnicodeString, int> recipe;
	typedef std::map <Corpus2::Tag, std::map<recipe, int> > tag_info;
	tag_info tags;
	
	Properties();
	void feedOn(const Properties & another);
};

std::ostream & operator<< (std::ostream& stream, const Properties & props);



class Tree : public Corpus2::BackwardTrie<Properties>
{
public:
	
	/**
	  Insert a word.
	  */
	void insert(const UnicodeString & orth, const UnicodeString & base, Corpus2::Tag tag);
	
	/**
	  Compress losslessly the tree, removing all nodes used only once, bar first ones.
	  */
	void compress();
	
	/**
	  Remove all nodes that are not intersting.
	  */
	void prune();
	
	
private:
	static bool compress(Node & node);
	
	void prune(Node & node) const;
	bool isBad(const Node &node) const;
	static void collect(Node & node);
	
	static Corpus2::Tag unmark(Corpus2::Tag tag);
};


#endif // TREE_H
