#ifndef SWIG_LIBCORPUS2_IOB_I
#define SWIG_LIBCORPUS2_IOB_I

%module libcorpusiob
%{
  #include <libcorpus2/ann/iob.h>
%}

// %include "...i"

namespace Corpus2 {
  namespace IOB {
    enum Enum {
      O = 0,
      B = 1,
      I = 2,
      PostLast
    };

    const char* to_string(Enum iob);
    Enum from_string(const std::string& s);
  }
}

using namespace std;
using namespace Corpus2::IOB;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_IOB_I */
