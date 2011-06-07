#ifndef SWIG_CORPUS2_I
#define SWIG_CORPUS2_I

%module corpus2

%include "boost_shared_ptr.i"
%include "unicodestring.i"
%include "exception.i"
%include "annotatedsentence.i"
%include "annotationchannel.i"
%include "annotationview.i"
%include "chunk.i"
%include "iob.i"
%include "lexeme.i"
%include "sentence.i"
%include "tag.i"
%include "tagset.i"
%include "tagsetmanager.i"
%include "token.i"
%include "tokenreader.i"
%include "tokenwriter.i"
%include "libpwrnlperror.i"

%{
#include <libcorpus2/util/settings.h>
static void set_verbose(bool v) {
        Corpus2::Path::Instance().set_verbose(v);
}
%}
static void set_verbose(bool);

#endif /* SWIG_CORPUS2_I */
