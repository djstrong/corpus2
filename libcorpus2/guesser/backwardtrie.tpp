#include <unicode/ustream.h>
#include <fstream>


namespace Corpus2 {


template <typename PropType>
BackwardTrie<PropType>::Node::Node(UChar character) :
    character(character)
{
	first_child = NULL;
	next_sibling = NULL;
}

template <typename PropType>
BackwardTrie<PropType>::Node::~Node()
{
	delete next_sibling;
	delete first_child;
}

template <typename PropType>
BackwardTrie<PropType>::BackwardTrie()
{
	
}




template <typename PropType>
typename BackwardTrie<PropType>::Node & BackwardTrie<PropType>::find_aux(BackwardTrie<PropType>::Node & node,
                                                 const UnicodeString &word,
                                                 boost::function<void(PropType &)> f, int i)
{
	if (f)
		f(node.properties);
	
	if (i < 0)
		return node;
	
	for (BackwardTrie<PropType>::Node * it = node.first_child; it != NULL; it = it->next_sibling)
		if (it->character == word[i])
			return find_aux(*it, word, f, i-1);
	return node;
}

template <typename PropType>
typename BackwardTrie<PropType>::Node & BackwardTrie<PropType>::find(const UnicodeString &word, boost::function<void(PropType &)> f)
{
	return find_aux(root, word, f, word.length()-1);
}

template <typename PropType>
const typename BackwardTrie<PropType>::Node & BackwardTrie<PropType>::find_aux(const BackwardTrie<PropType>::Node & node,
                                                 const UnicodeString &word,
                                                 boost::function<void(const PropType &)> f, int i) const
{
	if (f)
		f(node.properties);
	
	if (i < 0)
		return node;
	
	for (BackwardTrie<PropType>::Node * it = node.first_child; it != NULL; it = it->next_sibling)
		if (it->character == word[i])
			return find_aux(*it, word, f, i-1);
	return node;
}

template <typename PropType>
const typename BackwardTrie<PropType>::Node & BackwardTrie<PropType>::find(const UnicodeString &word, boost::function<void(const PropType &)> f) const
{
	return find_aux(root, word, f, word.length()-1);
}





template <typename PropType>
void BackwardTrie<PropType>::print() const
{
	root.print();
}

template <typename PropType>
void BackwardTrie<PropType>::Node::print(int d) const
{
	for (int i = 0; i < d; i++)
		std::cout << "  ";
	
	std::cout << UnicodeString(character) << " (" << properties << ")" << std::endl;
	
	for (Node * it = first_child; it != NULL; it = it->next_sibling)
		it->print(d+1);
}



template <typename PropType>
void BackwardTrie<PropType>::write(const boost::filesystem::path & path) const
{
	std::ofstream file(path.c_str());
	root.write(file);
}

template <typename PropType>
void BackwardTrie<PropType>::Node::write(std::ostream & stream) const
{
	stream << character << "\t";
	stream << properties << "\t";
	stream << childrenCount() << "\n";
	for (typename Corpus2::BackwardTrie<PropType>::Node * it = first_child; it != NULL; it = it->next_sibling)
		it->write(stream);
}


template <typename PropType>
void BackwardTrie<PropType>::read(const boost::filesystem::path & path)
{
	std::ifstream file(path.c_str());
	root.read(file);
}

template <typename PropType>
void BackwardTrie<PropType>::Node::read(std::istream & stream)
{
	stream >> character;
	stream >> properties;
	
	int chc;
	stream >> chc;
	Node ** it = &first_child;
	for (int i = 0; i < chc; i++)
	{
		*it = new Node();
		(*it)->read(stream);
		it = &((*it)->next_sibling);
	}
}




template <typename PropType>
int BackwardTrie<PropType>::Node::childrenCount() const
{
	Node * it = first_child;
	
	if (!it)
		return 0;
	
	int i = 1;
	
	while (it->next_sibling)
		it = it->next_sibling, ++i;
	return i;
}

template <typename PropType>
typename BackwardTrie<PropType>::Node * BackwardTrie<PropType>::Node::lastChild() const
{
	Node * it = first_child;
	
	if (!it)
		return it;
	
	while (it->next_sibling)
		it = it->next_sibling;
	return it;
}


} /* end ns Corpus2 */














