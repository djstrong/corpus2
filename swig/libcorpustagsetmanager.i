#ifndef SWIG_LIBCORPUS2_TAGSETMANAGER_I
#define SWIG_LIBCORPUS2_TAGSETMANAGER_I

%module libcorpustagsetmanager
%{
  #include <libcorpus2/tagsetmanager.h>
%}

%include "libcorpustagset.i"

%include "std_string.i"
%include "std_vector.i"

%feature("notabstract") Corpus2::TagsetManager;

namespace Corpus2 {
  class TagsetManager {
  public:
    TagsetManager();
    ~TagsetManager();

    /* --------------------------------------------------------------------- */
    const Tagset& get_tagset(const std::string& name);

    // boost::shared_ptr<Tagset> get_cache_entry(const std::string& name);

    /* --------------------------------------------------------------------- */
  };

  inline const Tagset& get_named_tagset(const std::string& name);
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_TAGSETMANAGER_I */
