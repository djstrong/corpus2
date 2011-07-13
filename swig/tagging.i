#ifndef SWIG_LIBCORPUS2_TAGGING_I
#define SWIG_LIBCORPUS2_TAGGING_I

%module libcorpustagging
%{
  #include <libcorpus2/tagging.h>
%}

%include "std_string.i"

%include "tag.i"
%include "tagset.i"
%include "token.i"

namespace Corpus2 {

Tag get_attribute_mask(const Tagset& tagset,
                       const std::string attr_name);

Tag mask_token(const Token& token, const Tag& mask, bool disamb_only);

}

using namespace std;
using namespace Corpus2;


#endif /* SWIG_LIBCORPUS2_TAGGING_I */
