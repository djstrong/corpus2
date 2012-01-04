#ifndef SWIG_LIBCORPUS2_RELATIONREADER_I
#define SWIG_LIBCORPUS2_RELATIONREADER_I

%module libcorpusrelationreader
%{
  #include <libcorpus2_whole/io/relreader.h>
%}

namespace Corpus2 {
namespace whole {
  class RelationReader {
  public:
    RelationReader(const std::string &rela_path);
    const std::vector< boost::shared_ptr<Relation> >& relations();
  };
} // whole ns
} // Corpus2 ns

using namespace std;
using namespace Corpus2;
using namespace Corpus2::whole;

#endif /* SWIG_LIBCORPUS2_RELATIONREADER_I */
