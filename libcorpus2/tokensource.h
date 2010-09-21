#ifndef LIBCORPUS2_TOKEN_SOURCE_H
#define LIBCORPUS2_TOKEN_SOURCE_H

#include <libpwrutils/tokensource.h>

namespace Corpus2 {

/// forward declaration
class Token;

/// typedef for a Corpus2 Token TokenSource, based on the general template
typedef PwrNlp::TokenSourceTemplate<Token> TokenSource;

/// pull RangeSource and make_range_source into our namespace
using PwrNlp::RangeSource;
using PwrNlp::make_range_source;

} /* end ns Corpus2 */

#endif // LIBCORPUS2_TOKEN_SOURCE_H
