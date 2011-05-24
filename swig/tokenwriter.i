#ifndef SWIG_LIBCORPUS2_TOKENWRITER_I
#define SWIG_LIBCORPUS2_TOKENWRITER_I

%module libcorpustokenwriter
%{
  #include <libcorpus2/io/writer.h>
%}

%include "tag.i"
%include "token.i"
%include "chunk.i"
%include "tagset.i"
%include "sentence.i"
%include "std_string.i"
%include "boost_shared_ptr.i"

%nodefaultctor Corpus2::TokenWriter;
%template(TokenWriterPtr) boost::shared_ptr<Corpus2::TokenWriter>;

namespace Corpus2 {
  class TokenWriter {
  public:
    typedef boost::shared_ptr<TokenWriter> TokenWriterPtr;
    /* --------------------------------------------------------------------- */

    TokenWriter();
    // TokenWriter(std::ostream& os, const Tagset& tagset, const string_range_vector& params);
    ~TokenWriter();
    
    /* --------------------------------------------------------------------- */
    virtual void write_token(const Token& t) = 0;
    void write_token_dispose(Token* t);
    virtual void write_sentence(const Sentence& s) = 0;
    virtual void write_chunk(const Chunk& p) = 0;

    /* --------------------------------------------------------------------- */
    // std::ostream& os();
    const Tagset& tagset();
    void finish();
    
    /* --------------------------------------------------------------------- */
    %exception {
      try {
        $action
      } catch (PwrNlp::PwrNlpError &e) {
        PyErr_SetString(PyExc_IndexError, e.info().c_str());
        return NULL;
      }
    }
    static TokenWriterPtr create_stream_writer(
      const std::string& class_id_params, 
      std::ostream& os, const Tagset& 
      tagset);

    %exception {
      try {
        $action
      } catch (PwrNlp::PwrNlpError &e) {
        PyErr_SetString(PyExc_IndexError, e.info().c_str());
        return NULL;
      }
    }
    static TokenWriterPtr create_path_writer(
      const std::string& class_id_params,
      const std::string& path,
      const Tagset& tagset);
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_TOKENWRITER_I */
