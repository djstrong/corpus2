#include <libcorpus2/io/statwriter.h>
#include <libcorpus2/ann/annotatedsentence.h>
#include <iomanip>
#include <libpwrutils/foreach.h>

namespace Corpus2 {

bool StatWriter::registered = TokenWriter::register_writer<StatWriter>(
	"stat");

StatWriter::StatWriter(std::ostream& os, const Tagset& tagset,
		const string_range_vector& params)
	: TokenWriter(os, tagset, params)
{
}

void StatWriter::write_token(const Token& t)
{
	if (t.orth().length() == 0) {
		os() << "~";
	} else {
		UnicodeString o1(t.orth().charAt(0));
		os() << PwrNlp::to_utf8(o1);
	}
}


void StatWriter::write_sentence(const Sentence& s)
{
	os() << std::setw(8) << "tokens" << " ";
	foreach (const Token* t, s.tokens()) {
		write_token(*t);
	}
	os() << "\n";
	const AnnotatedSentence* as = dynamic_cast<const AnnotatedSentence*>(&s);
	if (as) {
		foreach (const AnnotatedSentence::chan_map_t::value_type& vt, as->all_channels()) {
			os() << std::setw(8) << vt.first << " ";
			os() << vt.second.dump_alpha();
			os() << "\n";
		}
	}
	os() << "\n";
}

void StatWriter::write_chunk(const Chunk& c)
{
	foreach (const Sentence::Ptr s, c.sentences()) {
		write_sentence(*s);
	}
	os() << "\n";
}


} /* end ns Corpus2 */
