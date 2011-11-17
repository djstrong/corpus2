#ifndef SWIG_LIBCORPUS2_DOCUMENT_I
#define SWIG_LIBCORPUS2_DOCUMENT_I

%module libcorpusdocument
%{
  #include <libcorpus2/document.h>
%}

%include "std_defs.i"
%include "chunk.i"
%include "relation.i"

%template(DocumentPtr) boost::shared_ptr<Corpus2::Document>;
%template(ConstDocumentPtr) boost::shared_ptr<const Corpus2::Document>;

namespace Corpus2 {
  class Document {
  public:
    Document();
    ~Document();

    void add_paragraph(const boost::shared_ptr<Chunk> para);
    void add_relation(const boost::shared_ptr<Relation> relation);

    const std::vector< boost::shared_ptr<Chunk> >& paragraphs() const;
    const std::vector< boost::shared_ptr<Relation> >& relations() const;
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_DOCUMENT_I */
