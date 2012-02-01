#include "conllwriter.h"
#include <libpwrutils/foreach.h>
#include <boost/algorithm/string.hpp>


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
	boost::split(strs, tag, boost::is_any_of(":"));
	os()<<strs[0]<<"\t"<<strs[0]<<"\t";
	if(strs.size()>1)
	{
		size_t i;
		for(i=1;i<strs.size()-1;i++)
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

std::string ConllWriter::convert_tag(std::string tag)
{
	if(tag.compare("adja")==0||tag.compare("adjc")==0||tag.compare("adjp")==0||tag.compare("padj")==0||tag.compare("pact")==0||tag.compare("ppas")==0)
		return "adj";
	if(tag.compare("padv")==0||tag.compare("pant")==0||tag.compare("pcon")==0)
		return "adv";
	if(tag.compare("bedzie")==0||tag.compare("fin")==0||tag.compare("imps")==0||tag.compare("impt")==0||tag.compare("inf")==0||tag.compare("praet")==0||tag.compare("pred")==0||tag.compare("winien")==0)
		return "verb";
	if(tag.compare("psubst")==0||tag.compare("depr")==0||tag.compare("ger")==0||tag.compare("ppron3")==0||tag.compare("ppron12")==0||tag.compare("siebie")==0)
		return "subst";
	return tag;
}

} /* end ns Corpus2 */
