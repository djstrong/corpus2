#ifndef SWIG_LIBCORPUS2_RELATION_I
#define SWIG_LIBCORPUS2_RELATION_I

%module libcorpusrelation
%{
  #include <libcorpus2/relation.h>
%}

%include "std_defs.i"
%include "std_string.i"
%include "boost_shared_ptr.i"

%template(DirectionPointPtr) boost::shared_ptr<Corpus2::DirectionPoint>;
%template(ConstDirectionPointPtr) boost::shared_ptr<const Corpus2::DirectionPoint>;

%template(RelationPtr) boost::shared_ptr<Corpus2::Relation>;
%template(ConstRelationPtr) boost::shared_ptr<const Corpus2::Relation>;
%template(RelationPtrVector) std::vector< boost::shared_ptr<Corpus2::Relation> >;

namespace Corpus2 {
  class DirectionPoint {
  public:
    DirectionPoint(const std::string, const std::string, const int);

    const std::string sentence_id() const;
    const std::string channel_name() const;
    int annotation_number() const;
  };

  class Relation {
  public:
    /*
    Relation(const std::string& name,
       const boost::shared_ptr<const DirectionPoint> from,
       const boost::shared_ptr<const DirectionPoint> to);
    */
    Relation(const std::string& name,
       const DirectionPoint& from,
       const DirectionPoint& to);
    ~Relation();


    /* It must be renamed because "from" is python keyword */
    %rename(rel_from) from() const;
    const boost::shared_ptr<const DirectionPoint>& from() const;
    /* Also "to" for consistency should be renamed to rel_to */
    %rename(rel_to) to() const;
    const boost::shared_ptr<const DirectionPoint>& to() const;
    /* ...and "name" to rel_name */
    %rename(rel_name) name() const;
    const std::string name() const;
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_RELATION_I */
