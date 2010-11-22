/*
    Copyright (C) 2010 Tomasz Śniatowski, Adam Radziszewski
    Part of the libcorpus2 project

    This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.

    This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. 

    See the LICENSE and COPYING files for more details.
*/

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
