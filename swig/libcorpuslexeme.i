#ifndef SWIG_LIBCORPUS2_LEXEME_I
#define SWIG_LIBCORPUS2_LEXEME_I

%module libcorpuslexeme
%{
  #include <unicode/unistr.h>
  #include <libcorpus2/lexeme.h>
%}

%include "libcorpustag.i"

%include "std_string.i"
%include "std_vector.i"

%feature("notabstract") Corpus2::Lexeme;

%rename(__op_eq__) Corpus2::Lexeme::operator==(const Lexeme& other) const;
%rename(__op_lw__) Corpus2::Lexeme::operator<(const Lexeme& other) const;

namespace Corpus2 {
  class Lexeme {
  public:
    Lexeme();
    Lexeme(const UnicodeString& lemma, const Tag& tag);

    static Lexeme create(const UnicodeString& lemma, const Tag& tag);
    static Lexeme create_utf8(const std::string& lemma, const Tag& tag);

    const UnicodeString& lemma() const;
    const std::string lemma_utf8() const;

    void set_lemma(const UnicodeString& l);
    void set_lemma_utf8(const std::string& l);

    const Tag& tag() const;
    void set_tag(const Tag& tag);

    bool is_null() const;
    void set_disamb(bool value);
    bool is_disamb() const;

    bool operator<(const Lexeme& other) const;
    bool operator==(const Lexeme& other) const;
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_LEXEME_I */
