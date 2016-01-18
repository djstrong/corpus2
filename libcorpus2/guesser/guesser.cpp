#include <libcorpus2/guesser/guesser.h>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <libcorpus2/tagset.h>


namespace Corpus2 {


std::ostream & operator<< (std::ostream& stream, const detail::tags_info & props)
{
	foreach (detail::tags_info::value_type kv, props)
		stream << "[" << kv.first.get_pos() << "\t" << kv.first.get_values() << "\t" << kv.second.first << "\t" << kv.second.second << "]";
	
	return stream;
}

std::istream & operator>> (std::istream& stream, detail::tags_info & props)
{
	int tc;
	stream >> tc;
	
	for (int i = 0; i < tc; i++)
	{
		unsigned long pos, val;
		std::string r;
		int r2;
		stream >> pos >> val >> boost::io::quoted(r) >> r2;
		props[Tag(pos, val)] = std::make_pair(r.c_str(), r2);
	}
	
	return stream;
}



Guesser::Guesser(const boost::filesystem::path &path, const Tagset &tagset) :
    tagset(tagset)
{
	tree.read(path);
}

const detail::tags_info &Guesser::Tree::get(const UnicodeString &word) const
{
	return find(word).properties;
}

std::vector<Lexeme> Guesser::guess(const UnicodeString &word) const
{
	std::vector<Lexeme> res;
	
	const detail::tags_info & inf = tree.get(word);
	
	foreach (detail::tags_info::value_type kv, inf)
	{
		UnicodeString lemma = word.tempSubString(0, word.length() - kv.second.second) + kv.second.first;
		res.push_back(Lexeme(lemma, mark(kv.first, word)));
	}
	
	return res;
}

void Guesser::print() const
{
	tree.print();
}

Tag Guesser::mark(Tag tag, const UnicodeString &word) const
{
	const Corpus2::mask_t sup_mask = tagset.get_value_mask(std::string("sup"));
	const Corpus2::mask_t com_mask = tagset.get_value_mask(std::string("com"));
	const Corpus2::mask_t aff_mask = tagset.get_value_mask(std::string("aff"));
	const Corpus2::mask_t neg_mask = tagset.get_value_mask(std::string("neg"));
	
	if (tag.get_values_for(com_mask).any() && word.startsWith("naj"))
	{
		Corpus2::mask_t v = ((tag.get_values()) & ~com_mask) | sup_mask;
		tag.set_values(v);
	}
	
	if (tag.get_values_for(aff_mask).any() && word.startsWith("nie"))
	{
		Corpus2::mask_t v = ((tag.get_values()) & ~aff_mask) | neg_mask;
		tag.set_values(v);
	}
	
	return tag;
}

}
