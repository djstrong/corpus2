#include <libcorpus2/guesser/guesser.h>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <libcorpus2/tagset.h>


namespace Corpus2 {



void detail::tags_info::print(std::ostream &stream) const
{
	foreach (value_type kv, *this)
		stream << "[" << kv.first.get_pos() << "\t" << kv.first.get_values() << "\t" << kv.second.first << "\t" << kv.second.second << "]";
}

void detail::tags_info::dedump(std::istream &stream)
{
	int tc;
	stream >> tc;
	
	for (int i = 0; i < tc; i++)
	{
		unsigned long pos, val;
		std::string r;
		int r2;
		stream >> pos >> val >> boost::io::quoted(r) >> r2;
		(*this)[Tag(pos, val)] = std::make_pair(r.c_str(), r2);
	}
}



Guesser::Guesser(const boost::filesystem::path &path, const Tagset &tagset) :
    tagset(tagset)
{
	tree.read(path);
}


void get_aux(const detail::tags_info ** last_good, const detail::tags_info & prop)
{
	if (!prop.empty())
		*last_good = &prop;
}

const detail::tags_info &Guesser::Tree::get(const UnicodeString &word) const
{
	const detail::tags_info * last_good = &root.properties;
	find(word, bind(&get_aux, &last_good, _1));
	return *last_good;
}

std::vector<Lexeme> Guesser::guess(const UnicodeString &word) const
{
	std::vector<Lexeme> res;
	
	const detail::tags_info & inf = tree.get(word);
	
	foreach (detail::tags_info::value_type kv, inf)
	{
		UnicodeString lemma = word.tempSubString(0, word.length() - kv.second.second) + kv.second.first;
		res.push_back(mark(lemma, kv.first));
	}
	
	return res;
}

void Guesser::print() const
{
	tree.print();
}

Corpus2::Lexeme Guesser::mark(UnicodeString halflemma, Tag tag) const
{
	const Corpus2::mask_t sup_mask = tagset.get_value_mask(std::string("sup"));
	const Corpus2::mask_t com_mask = tagset.get_value_mask(std::string("com"));
	const Corpus2::mask_t aff_mask = tagset.get_value_mask(std::string("aff"));
	const Corpus2::mask_t neg_mask = tagset.get_value_mask(std::string("neg"));
	
	if (tag.get_values_for(com_mask).any() && halflemma.startsWith("naj"))
	{
		Corpus2::mask_t v = ((tag.get_values()) & ~com_mask) | sup_mask;
		tag.set_values(v);
		halflemma.remove(0, 3);
	}
	
	if (tag.get_values_for(aff_mask).any() && halflemma.startsWith("nie"))
	{
		Corpus2::mask_t v = ((tag.get_values()) & ~aff_mask) | neg_mask;
		tag.set_values(v);
		halflemma.remove(0, 3);
	}
	
	return Lexeme(halflemma, tag);
}

}
