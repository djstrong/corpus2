#ifndef SWIG_LIBCORPUS2_TAGSET_I
#define SWIG_LIBCORPUS2_TAGSET_I

%module libcorpustagset
%{
  #include <libcorpus2/tagset.h>
%}

%include "libcorpustag.i"

%include "std_string.i"
%include "std_vector.i"

%feature("notabstract") Corpus2::Tagset;

namespace Corpus2 {
  class Tagset {
  public:

    enum ParseMode {
      ParseCheckRequired = 1,
      ParseAllowExtra = 2,
      ParseCheckSingular = 4,

      ParseDefault = ParseCheckRequired, 
      ParseRequiredWithExtra = ParseCheckRequired | ParseAllowExtra,
      ParseStrict = ParseCheckRequired | ParseCheckSingular,
      ParseLoose = ParseAllowExtra
    };

    Tagset();
    static Tagset from_data(const char*);

    /* --------------------------------------------------------------------- */
    Tag parse_symbol(const std::string& s) const;
    Tag parse_symbol_string(const std::string& s) const;

    /* --------------------------------------------------------------------- */
    std::vector<Tag> parse_tag(const char* c, ParseMode m = ParseDefault) const;
    Tag parse_simple_tag(const char* c, ParseMode mode = ParseDefault) const;
    Tag make_ign_tag() const;

    /* --------------------------------------------------------------------- */
    std::string tag_to_string(const Tag& tag) const;
    std::string tag_to_no_opt_string(const Tag &tag) const;

    /* --------------------------------------------------------------------- */
    std::vector<std::string> tag_to_symbol_string_vector(const Tag& tag, bool 
      compress_attribites = true) const;

    std::string tag_to_symbol_string(const Tag& tag,
      bool compress_attribites = true) const;

    /* --------------------------------------------------------------------- */
    size_t tag_size(const Tag& tag) const;
    bool tag_is_singular(const Tag& tag) const;

    /* --------------------------------------------------------------------- */
    std::vector<Tag> split_tag(const Tag& tag) const;

    /* --------------------------------------------------------------------- */
    int pos_count() const;
    int attribute_count() const;
    int value_count() const;
    size_t size() const;
    
    double size_extra() const;

    /* --------------------------------------------------------------------- */
    std::string name() const;
    void set_name(const std::string& name);


    std::string id_string() const;
    std::string id_string(const Tag& tag) const;
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_TAGSET_I */
