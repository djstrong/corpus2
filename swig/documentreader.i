#ifndef SWIG_LIBCORPUS2_DOCUMENTREADER_I
#define SWIG_LIBCORPUS2_DOCUMENTREADER_I

%module libcorpusdocumentreader
%{
  #include <libcorpus2/io/docreader.h>
%}

%include "document.i"
%include "boost_shared_ptr.i"

namespace Corpus2 {
  class DocumentReader {
  public:
    DocumentReader(const Tagset&, const std::string&, const std::string &);
    boost::shared_ptr<Document> read();

    /* --------------------------------------------------------------------- */
    ~DocumentReader();
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_DOCUMENTREADER_I */
