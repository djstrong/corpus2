#include <libcorpus2/io/cclwriter.h>
#include <libpwrutils/foreach.h>
#include <libcorpus2/ann/annotatedsentence.h>
#include <libcorpus2/io/xcescommon.h>
#include <libcorpus2/tokenmetadata.h>

namespace Corpus2 {

bool CclWriter::registered = TokenWriter::register_writer<CclWriter>("ccl",
		"flat,chunk,nochunk,nodisamb,sorttags,split,ws");

CclWriter::CclWriter(std::ostream& os, const Tagset& tagset,
		const string_range_vector& params)
	: XmlWriter(os, tagset, params)
{
	do_header();
}

CclWriter::~CclWriter()
{
	finish();
}

void CclWriter::write_sentence(const Sentence& s)
{
	paragraph_head();
	if (use_indent_) indent_more();
	write_sentence_int(s);
	if (use_indent_) indent_less();
	osi() << "</chunk>\n";
}

void CclWriter::write_sentence_int(const Sentence &s)
{
	const AnnotatedSentence* ann = dynamic_cast<const AnnotatedSentence*>(&s);
	osi() << "<sentence>\n";
	if (use_indent_) indent_more();
	for (size_t idx = 0; idx < s.size(); ++idx) {
		const Token* t = s.tokens()[idx];
		if (ann) {
			token_as_xces_xml_head(os(), *t, use_indent_ ? indent_level() : -1, whitespace_info_);
			if (use_indent_) indent_more();
			token_as_xces_xml_body(os(), tagset(), *t, use_indent_ ? indent_level() : -1, output_disamb_, sort_tags_);
			foreach (const AnnotatedSentence::chan_map_t::value_type& v, ann->all_channels()) {
				osi() << "<ann chan=\"" << v.first << "\"";
				if (v.second.is_head_at(idx)) {
					os() << " head=\"1\"";
				}
				os() << ">";
				os() << v.second.get_segment_at(idx);
				os() << "</ann>\n";
			}
			TokenMetaData* md = t->get_metadata();
			if (md) {
				foreach (const TokenMetaData::attr_map_t::value_type& v, md->attributes()) {
					osi() << "<prop key=\"" << v.first << "\"" << ">";
					os() << v.second << "</prop>\n";
				}
			}
			if (use_indent_) indent_less();
			osi() << "</tok>\n";
		} else {
			// TODO: currently writing of token metadata is supported only when
			// we've got an AnnotatedSentence.
			XmlWriter::write_token(*t);
		}
	}
	if (use_indent_) indent_less();	osi() << "</sentence>\n";
}

void CclWriter::write_chunk(const Chunk &c)
{
	paragraph_head(c);
	if (use_indent_) indent_more();
	foreach (const Sentence::ConstPtr& s, c.sentences()) {
		write_sentence_int(*s);
	}
	if (use_indent_) indent_less();
	osi() << "</chunk>\n";
}

void CclWriter::do_header()
{
	XmlWriter::do_header();
	os() << "<!DOCTYPE cesAna SYSTEM \"xcesAnaIPI.dtd\">\n";
	os() << "<chunkList";
	os() << " xmlns:xlink=\"http://www.w3.org/1999/xlink\"";
	os() << ">\n";
	if (use_indent_) indent_more();
}

void CclWriter::do_footer()
{
	if (use_indent_) indent_less();
	os() << "</chunkList>\n";
}

void CclWriter::paragraph_head()
{
	osi() << "<chunk id=\"ch" << ++cid_ << "\""
		<< " type=\"p\">\n";
}

void CclWriter::paragraph_head(const Chunk& c)
{
	osi() << "<chunk";
	foreach (const Chunk::attr_map_t::value_type& v, c.attributes()) {
		os() << " " << v.first << "=\"" << v.second << "\"";
	}
	os() << ">\n";
}

} /* end ns Corpus2 */
