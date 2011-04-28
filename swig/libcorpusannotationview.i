#ifndef SWIG_LIBCORPUS2_ANNOTATIONVIEW_I
#define SWIG_LIBCORPUS2_ANNOTATIONVIEW_I

%module libcorpusannotationview
%{
  #include <libcorpus2/ann/view.h>
%}

%include "libcorpussentence.i"
%include "libcorpusannotationchannel.i"
%include "libcorpusannotatedsentence.i"

%include "std_string.i"
%include "boost_shared_ptr.i"

// %template(AnnotatedSentencePtr) boost::shared_ptr<AnnotatedSentence>;

%nodefaultctor Corpus2::TokenWriter;

namespace Corpus2 {
  class AnnotationView : public Corpus2::Sentence {
  public:
    AnnotationView(const boost::shared_ptr<AnnotatedSentence>& original, const std::string& ann_name_);
    ~AnnotationView();

    /* --------------------------------------------------------------------- */
    Ptr clone_shared() const;
    
    /* --------------------------------------------------------------------- */
    void commit();
    void commit_to(
      const boost::shared_ptr<AnnotatedSentence>& original, 
      const std::string& ann_name);

    /* --------------------------------------------------------------------- */
    void release_original();
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_ANNOTATIONVIEW_I */
