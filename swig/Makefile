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

CBIN=libcorpustag.o \
		 libcorpustagset.o \
		 libcorpustagsetmanager.o \
		 libcorpuslexeme.o \
		 libcorpussentence.o \
		 libcorpuschunk.o \
		 libcorpustokenwriter.o \
		 libcorpustokenreader.o

CBINOUT=_boost_shared_ptr.so \
				_libcorpustag.so \
				_libcorpustagset.so \
				_libcorpustagsetmanager.so \
				_libcorpuslexeme.so \
				_libcorpustoken.so \
				_libcorpussentence.so \
				_libcorpuschunk.so \
				_libcorpustokenwriter.so \
				_libcorpustokenreader.so

CWRAP=boost_shared_ptr_wrap.cxx \
			libcorpustag_wrap.cxx \
			libcorpustagset_wrap.cxx \
			libcorpustagsetmanager_wrap.cxx \
			libcorpuslexeme_wrap.cxx \
			libcorpustoken_wrap.cxx \
			libcorpussentence_wrap.cxx \
			libcorpuschunk_wrap.cxx \
			libcorpustokenwriter_wrap.cxx \
			libcorpustokenreader_wrap.cxx

CWRAPBIN=boost_shared_ptr_wrap.o \
				 libcorpustag_wrap.o \
				 libcorpustagset_wrap.o \
				 libcorpustagsetmanager_wrap.o \
				 libcorpuslexeme_wrap.o \
				 libcorpustoken_wrap.o \
				 libcorpussentence_wrap.o \
				 libcorpuschunk_wrap.o \
				 libcorpustokenwriter_wrap.o \
				 libcorpustokenreader_wrap.o

PYMODULES=boost_shared_ptr.py \
					libcorpustag.py \
					libcorpustagset.py \
					libcorpustagsetmanager.py \
					libcorpuslexeme.py \
					libcorpustoken.py \
					libcorpussentence.py \
					libcorpuschunk.py \
					libcorpustokenwriter.py \
					libcorpustokenreader.py

PYCBIN=boost_shared_ptr.pyc \
			 libcorpustag.pyc \
			 libcorpustagset.pyc \
			 libcorpustagsetmanager.pyc \
			 libcorpuslexeme.pyc \
			 libcorpustoken.pyc \
			 libcorpussentence.pyc \
			 libcorpuschunk.pyc \
			 libcorpustokenwriter.pyc \
			 libcorpustokenreader.pyc

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

# Tagset
libcorpustagset.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpustagset.i
	$(CPP) -c libcorpustagset_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpustagset_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpustagset.so

# TagsetManager
libcorpustagsetmanager.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpustagsetmanager.i
	$(CPP) -c libcorpustagsetmanager_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpustagsetmanager_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpustagsetmanager.so

# Lexeme
libcorpuslexeme.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpuslexeme.i
	$(CPP) -c libcorpuslexeme_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpuslexeme_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpuslexeme.so

# Chunk
libcorpuschunk.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpuschunk.i
	$(CPP) -c libcorpuschunk_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpuschunk_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpuschunk.so

# Sentence
libcorpussentence.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpussentence.i
	$(CPP) -c libcorpussentence_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpussentence_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpussentence.so

# TokenWriter
libcorpustokenwriter.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpustokenwriter.i
	$(CPP) -c libcorpustokenwriter_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpustokenwriter_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpustokenwriter.so

# TokenReader
libcorpustokenreader.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpustokenreader.i
	$(CPP) -c libcorpustokenreader_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpustokenreader_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpustokenreader.so

# -----------------------------------------------------------------------------
clean:
	rm -f $(CBIN) $(CBINOUT) $(CWRAP) $(CWRAPBIN) $(PYMODULES) $(PYCBIN)
