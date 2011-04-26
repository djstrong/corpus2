/**
 * Wrapper for boost shared_ptr
 */
#ifndef SWIG_BOOST_SHAREDPTR
#define SWIG_BOOST_SHAREDPTR

%module boost_shared_ptr
%{
%}

namespace boost {
  template<class T> class shared_ptr {
  public:
    shared_ptr();
    shared_ptr(T * p);
    T* operator->();
  private:
    T * px;
    int pn;
  };
}

using namespace boost;

#endif /* SWIG_BOOST_SHAREDPTR */
