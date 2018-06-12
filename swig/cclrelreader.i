#ifndef SWIG_LIBCORPUS2_DOCUMENTREADER_I
#define SWIG_LIBCORPUS2_DOCUMENTREADER_I

%module libcorpuscclrelreader
%{
  #include <libcorpus2_whole/io/cclrelreader.h>
%}

%include <exception.i>
%include "corpus2exception.i"
%include "document.i"
%include "boost_shared_ptr.i"


%feature("autodoc", "1");
namespace Corpus2 {
namespace whole {
  class CclRelReader {
  public:
    %exception {
      try {
        $action
      } catch (PwrNlp::PwrNlpError &e) {
        PyErr_SetString(PyExc_IndexError, e.info().c_str());
        return NULL;
      }
    }
    CclRelReader(const Tagset&, const std::string&, const std::string &);

    %exception {
      try {
        $action
      } catch (PwrNlp::PwrNlpError &e) {
        PyErr_SetString(PyExc_IndexError, e.info().c_str());
        return NULL;
      } catch (xmlpp::parse_error &e) {
        std::string s("xmlpp::parse_error: "), s2(e.what());
        s = s + s2;
        SWIG_exception(SWIG_RuntimeError, s.c_str());
      }
    }
    boost::shared_ptr<Document> read();

    void set_option(const std::string& option);
    std::string get_option(const std::string& option) const;

    /* --------------------------------------------------------------------- */
    ~CclRelReader();
  };
} // whole ns
} // Corpus2 ns

using namespace std;
using namespace Corpus2;
using namespace Corpus2::whole;

#endif /* SWIG_LIBCORPUS2_DOCUMENTREADER_I */
