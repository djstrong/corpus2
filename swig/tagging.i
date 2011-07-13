#ifndef SWIG_LIBCORPUS2_TAGGING_I
#define SWIG_LIBCORPUS2_TAGGING_I

%module libcorpustagging
%{
  #include <libcorpus2/tagging.h>
%}

%include "std_string.i"

%include "tag.i"
%include "tagset.i"

namespace Corpus2 {

Tag get_attribute_mask(const Tagset& tagset,
					   const std::string attr_name);
}

using namespace std;
using namespace Corpus2;


#endif /* SWIG_LIBCORPUS2_TAGGING_I */
