#ifndef SWIG_LIBCORPUS2_TOKEN_I
#define SWIG_LIBCORPUS2_TOKEN_I

%module libcorpustoken
%{
  #include <unicode/unistr.h>
  #include <libcorpus2/token.h>
%}

%include "libcorpuslexeme.i"
%include "libcorpustagset.i"

%include "std_pair.i"
%include "std_string.i"
%include "std_vector.i"

%feature("notabstract") Corpus2::Token;

%rename(__op_eq__) Corpus2::Token::operator==(const Token& other) const;

%template(lexemeVector) std::vector<Lexeme>;

namespace Corpus2 {
  class Token {
  public:
    Token();
    Token(const UnicodeString& orth, PwrNlp::Whitespace::Enum wa);
    Token* clone() const;
    
    const UnicodeString& orth() const;
    std::string orth_utf8() const;

    void set_orth(const UnicodeString& orth);
    // void set_orth_utf8(const std::string& orth);

    const PwrNlp::Whitespace::Enum& wa() const;
    void set_wa(const PwrNlp::Whitespace::Enum& wa);

    const std::vector<Lexeme>& lexemes() const;
    std::vector<Lexeme>& lexemes();

    void add_lexeme(const Lexeme& lex);

    const Lexeme& get_preferred_lexeme(const Tagset& tagset) const;
    size_t get_preferred_lexeme_index(const Tagset& tagset) const;

    void make_ign(const Tagset& tagset);
    void add_ign(const Tagset& tagset);

    bool operator==(const Token& other) const;

    bool check_duplicate_lexemes() const;
    bool remove_duplicate_lexemes();

    bool has_disamb_lexeme() const;
    int count_disamb_lexemes() const;
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_TOKEN_I */
