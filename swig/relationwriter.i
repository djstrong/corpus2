#ifndef SWIG_LIBCORPUS2_RELATIONWRITER_I
#define SWIG_LIBCORPUS2_RELATIONWRITER_I

%module libcorpusrelationwriter
%{
  #include <libcorpus2_whole/io/relwriter.h>
%}


namespace Corpus2 {
namespace whole {
  class RelationWriter {
  public:
    RelationWriter(const std::string &rela_path);
    void write(const std::vector< boost::shared_ptr<Relation> > relations);
  private:
    void do_header();
    void do_footer();
    //void write_relation(const boost::shared_ptr<Relation>& r);
};
} // whole ns
} // Corpus2 ns

using namespace std;
using namespace Corpus2;
using namespace Corpus2::whole;

#endif /* SWIG_LIBCORPUS2_RELATIONWRITER_I */

