#ifndef LIBCORPUS2_SENTENCE_H
#define LIBCORPUS2_SENTENCE_H

#include <libcorpus2/token.h>

#include <libpwrutils/sentence.h>

namespace Corpus2 {

/// typedef for a sentence of Corpus2 tokens based on the PwrNlp sentence
/// template.
typedef PwrNlp::SentenceTemplate<Token> Sentence;

} /* end ns Corpus2 */

#endif // LIBCORPUS2_SENTENCE_H
