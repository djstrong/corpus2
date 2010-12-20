#ifndef LIBCORPUS2_ANN_IOB_H
#define LIBCORPUS2_ANN_IOB_H

#include <string>

namespace Corpus2 {

namespace IOB {
	/**
	 * A simple enumeration for IOB annotation chunk tagging
	 */
	enum Enum {
		O = 0,
		B = 1,
		I = 2,
		PostLast
	};
	/// Convert an enum value to an uppercase I, O or B string
	const char* to_string(Enum iob);
	/// Create an enum value from an I, O or B string
	Enum from_string(const std::string& s);
}

} /* end ns Corpus2 */

#endif // LIBCORPUS2_ANN_IOB_H
