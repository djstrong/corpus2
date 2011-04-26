CPP=g++
SWIG=swig
SWIGOPTS_LANG=-c++ -python 

PYTHONDIR=/usr/include/python2.6
WCCLDIR=/usr/local/include/libwccl/
CORPUS2DIR=/usr/local/include/libcorpus2/
PWRUTILDIR=

CORPUS2BIN=/usr/local/lib/libcorpus2.so
PWRUTILBIN=/usr/local/lib/libpwrutils.so
ANTLRLIB=/usr/lib/libantlr-pic.a 

CPPFLAGS=-fPIC -O2

CBIN=libcorpustag.o

CBINOUT=_boost_shared_ptr.so \
				_libcorpustag.so

CWRAP=boost_shared_ptr_wrap.cxx \
			libcorpustag_wrap.cxx

CWRAPBIN=boost_shared_ptr_wrap.o \
				 libcorpustag_wrap.o

PYMODULES=boost_shared_ptr.py \
					libcorpustag.py

PYCBIN=boost_shared_ptr.pyc \
			 libcorpustag.pyc

# -----------------------------------------------------------------------------
all:boost_shared_ptr.o $(CBIN)
	# Done
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
#  boost::shared_ptr wrappers
# -----------------------------------------------------------------------------
# shared_ptr
boost_shared_ptr.o:
	$(SWIG) $(SWIGOPTS_LANG) boost_shared_ptr.i
	$(CPP) -c boost_shared_ptr_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared boost_shared_ptr_wrap.o -o _boost_shared_ptr.so


# -----------------------------------------------------------------------------
#  Corpus2 Wrappers
# -----------------------------------------------------------------------------
# Tag
libcorpustag.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpustag.i
	$(CPP) -c libcorpustag_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpustag_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpustag.so

# -----------------------------------------------------------------------------
clean:
	rm -f $(CBIN) $(CBINOUT) $(CWRAP) $(CWRAPBIN) $(PYMODULES) $(PYCBIN)
