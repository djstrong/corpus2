#include "conllwriter.h"
#include <libpwrutils/foreach.h>
#include <boost/algorithm/string.hpp>
#include <algorithm>


namespace Corpus2 {

bool ConllWriter::registered = TokenWriter::register_writer<ConllWriter>("conll");

ConllWriter::ConllWriter(std::ostream& os, const Tagset& tagset,
		const string_range_vector& params)
	: TokenWriter(os, tagset, params)
{
	myTagset=tagset;
}

ConllWriter::~ConllWriter()
{
	finish();
}

void ConllWriter::write_token(const Token &t)
{
	os()<<t.orth_utf8()<<"\t";
	Lexeme lex = t.get_preferred_lexeme(myTagset);
	os()<<lex.lemma_utf8()+"\t";
	std::string tag = myTagset.tag_to_string(lex.tag());
	std::vector<std::string> strs;
	std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
	boost::split(strs, tag, boost::is_any_of(":"));
	os()<<strs[1]<<"\t"<<strs[0]<<"\t";
	if(strs.size()>2)
	{
		size_t i;
		for(i=2;i<strs.size()-1;i++)
		{
			os()<<strs[i]<<"|";
		}
		os()<<strs[i]<<"\t_\t_\t_\t_";
	}
	else
		os()<<"_\t_\t_\t_\t_";
}

void ConllWriter::write_sentence(const Sentence& s)
{
	int i=1;
	foreach (const Token* t, s.tokens()) {
		os()<<i<<"\t";
		write_token(*t);
		os()<<"\n";
		i++;
	}
	os()<<"\n";
}

void ConllWriter::write_chunk(const Chunk &c)
{
	foreach (const Sentence::ConstPtr& s, c.sentences()) {
		write_sentence(*s);
	}
}

void ConllWriter::do_header()
{

}

void ConllWriter::do_footer()
{
}

} /* end ns Corpus2 */
