#ifndef SWIG_LIBCORPUS2_RELATIONREADER_I
#define SWIG_LIBCORPUS2_RELATIONREADER_I

%module libcorpusrelationreader
%{
  #include <libcorpus2_whole/relreader.h>
%}

namespace Corpus2 {
  class RelationReader {
  public:
    RelationReader(const std::string &rela_path);
    const std::vector< boost::shared_ptr<Relation> >& relations();
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_RELATIONREADER_I */
