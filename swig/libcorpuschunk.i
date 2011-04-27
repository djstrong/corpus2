#ifndef SWIG_LIBCORPUS2_CHUNK_I
#define SWIG_LIBCORPUS2_CHUNK_I

%module libcorpuschunk
%{
  #include <libcorpus2/chunk.h>
%}


%include "std_set.i"
%include "std_map.i"
%include "std_pair.i"
%include "std_string.i"
%include "std_vector.i"
%include "boost_shared_ptr.i"

%include "libcorpussentence.i"

%template(attr_map_t) std::map<std::string, std::string>;

namespace Corpus2 {
  class Chunk {
  public:
    Chunk();
    ~Chunk();

    /* --------------------------------------------------------------------- */

    boost::shared_ptr<Chunk> clone_shared() const;

    bool empty() const;
    size_t size() const;

    bool has_attribute(const std::string& name) const;
    std::string get_attribute(const std::string& name) const;
    void set_attribute(const std::string& name, const std::string& value);

    std::vector< boost::shared_ptr<Sentence> >& sentences();
    const std::vector< boost::shared_ptr<Sentence> >& sentences() const;
    void append(const boost::shared_ptr<Sentence>& s);

    typedef std::map<std::string, std::string> attr_map_t;
    const attr_map_t& attributes() const;
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_CHUNK_I */