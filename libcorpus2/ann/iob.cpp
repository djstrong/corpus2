#include <libcorpus2/ann/iob.h>

namespace Corpus2 {

const char* IOB::to_string(IOB::Enum iob)
{
	if (iob == IOB::I) return "I";
	if (iob == IOB::O) return "O";
	if (iob == IOB::B) return "B";
	return "?";
}

IOB::Enum IOB::from_string(const std::string &s)
{
	if (s == "I") return IOB::I;
	if (s == "O") return IOB::O;
	if (s == "B") return IOB::B;
	return IOB::PostLast;
}

} /* end ns Corpus2 */
