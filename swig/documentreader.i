#ifndef SWIG_LIBCORPUS2_DOCUMENTREADER_I
#define SWIG_LIBCORPUS2_DOCUMENTREADER_I

%module libcorpusdocumentreader
%{
  #include <libcorpus2/io/docreader.h>
%}

%include "exception.i"
%include "document.i"
%include "boost_shared_ptr.i"

namespace Corpus2 {
  class DocumentReader {
  public:
    %exception {
      try {
        $action
      } catch (PwrNlp::PwrNlpError &e) {
        PyErr_SetString(PyExc_IndexError, e.info().c_str());
        return NULL;
      }
    }
    DocumentReader(const Tagset&, const std::string&, const std::string &);

    %exception {
      try {
        $action
      } catch (PwrNlp::PwrNlpError &e) {
        PyErr_SetString(PyExc_IndexError, e.info().c_str());
        return NULL;
      }
    }
    boost::shared_ptr<Document> read();

    void set_option(const std::string& option);
    std::string get_option(const std::string& option) const;

    /* --------------------------------------------------------------------- */
    ~DocumentReader();
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_DOCUMENTREADER_I */
