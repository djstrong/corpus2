#ifndef SWIG_LIBCORPUS2_TAGSETMANAGER_I
#define SWIG_LIBCORPUS2_TAGSETMANAGER_I

%module libcorpustagsetmanager
%{
  #include <libcorpus2/tagsetmanager.h>
%}

%include "libcorpustagset.i"
%include "libcorpus2exception.i"

%include "std_string.i"
%include "std_vector.i"
%include "boost_shared_ptr.i"

%feature("notabstract") Corpus2::TagsetManager;
%template(TagsetPtr) boost::shared_ptr<Tagset>;

namespace Corpus2 {
  class TagsetManager {
  public:
    TagsetManager();
    ~TagsetManager();

    /* --------------------------------------------------------------------- */
    const Tagset& get_tagset(const std::string& name);
    boost::shared_ptr<Tagset> get_cache_entry(const std::string& name);

    /* --------------------------------------------------------------------- */
  };

  inline const Tagset& get_named_tagset(const std::string& name);
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_TAGSETMANAGER_I */
