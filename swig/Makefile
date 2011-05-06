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

CBIN=libpwrnlperror.o \
		 libcorpus2exception.o \
		 libcorpustag.o \
		 libcorpustagset.o \
		 libcorpustagsetmanager.o \
		 libcorpuslexeme.o \
		 libcorpussentence.o \
		 libcorpuschunk.o \
		 libcorpustoken.o \
		 libcorpustokenwriter.o \
		 libcorpustokenreader.o \
		 libcorpusiob.o \
		 libcorpusannotationchannel.o \
		 libcorpusannotatedsentence.o \
		 libcorpusannotationview.o

CBINOUT=_libpwrnlperror.so \
				_libcorpus2exception.so \
				_boost_shared_ptr.so \
				_libcorpustag.so \
				_libcorpustagset.so \
				_libcorpustagsetmanager.so \
				_libcorpuslexeme.so \
				_libcorpustoken.so \
				_libcorpussentence.so \
				_libcorpuschunk.so \
				_libcorpustokenwriter.so \
				_libcorpustokenreader.so \
				_libcorpusiob.so \
				_libcorpusannotationchannel.so \
				_libcorpusannotatedsentence.so \
				_libcorpusannotationview.so

CWRAP=libpwrnlperror_wrap.cxx \
			libcorpus2exception_wrap.cxx \
			boost_shared_ptr_wrap.cxx \
			libcorpustag_wrap.cxx \
			libcorpustagset_wrap.cxx \
			libcorpustagsetmanager_wrap.cxx \
			libcorpuslexeme_wrap.cxx \
			libcorpustoken_wrap.cxx \
			libcorpussentence_wrap.cxx \
			libcorpuschunk_wrap.cxx \
			libcorpustokenwriter_wrap.cxx \
			libcorpustokenreader_wrap.cxx \
			libcorpusiob_wrap.cxx \
			libcorpusannotationchannel_wrap.cxx \
			libcorpusannotatedsentence_wrap.cxx \
			libcorpusannotationview_wrap.cxx

CWRAPBIN=libpwrnlperror_wrap.o \
				 libcorpus2exception_wrap.o \
				 boost_shared_ptr_wrap.o \
				 libcorpustag_wrap.o \
				 libcorpustagset_wrap.o \
				 libcorpustagsetmanager_wrap.o \
				 libcorpuslexeme_wrap.o \
				 libcorpustoken_wrap.o \
				 libcorpussentence_wrap.o \
				 libcorpuschunk_wrap.o \
				 libcorpustokenwriter_wrap.o \
				 libcorpustokenreader_wrap.o \
				 libcorpusiob_wrap.o \
				 libcorpusannotationchannel_wrap.o \
				 libcorpusannotatedsentence_wrap.o \
				 libcorpusannotationview_wrap.o

PYMODULES=libpwrnlperror.py \
					libcorpus2exception.py \
					boost_shared_ptr.py \
					libcorpustag.py \
					libcorpustagset.py \
					libcorpustagsetmanager.py \
					libcorpuslexeme.py \
					libcorpustoken.py \
					libcorpussentence.py \
					libcorpuschunk.py \
					libcorpustokenwriter.py \
					libcorpustokenreader.py \
					libcorpusiob.py \
					libcorpusannotationchannel.py \
					libcorpusannotatedsentence.py \
					libcorpusannotationview.py

PYCBIN=libpwrnlperror.pyc \
			 libcorpus2exception.pyc \
			 boost_shared_ptr.pyc \
			 libcorpustag.pyc \
			 libcorpustagset.pyc \
			 libcorpustagsetmanager.pyc \
			 libcorpuslexeme.pyc \
			 libcorpustoken.pyc \
			 libcorpussentence.pyc \
			 libcorpuschunk.pyc \
			 libcorpustokenwriter.pyc \
			 libcorpustokenreader.pyc \
			 libcorpusiob.pyc \
			 libcorpusannotationchannel.pyc \
			 libcorpusannotatedsentence.pyc \
			 libcorpusannotationview.pyc

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
#  PwrNlpError wprapper
# -----------------------------------------------------------------------------
# PwrNlpError
libpwrnlperror.o:
	$(SWIG) $(SWIGOPTS_LANG) libpwrnlperror.i
	$(CPP) -c libpwrnlperror_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libpwrnlperror_wrap.o -o _libpwrnlperror.so

# -----------------------------------------------------------------------------
#  Corpus2 Wrappers
# -----------------------------------------------------------------------------

libcorpus2exception.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpus2exception.i
	$(CPP) -c libcorpus2exception_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpus2exception_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpus2exception.so

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

# Token
libcorpustoken.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpustoken.i
	$(CPP) -c libcorpustoken_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpustoken_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpustoken.so

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

# IOB
libcorpusiob.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpusiob.i
	$(CPP) -c libcorpusiob_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpusiob_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpusiob.so

# AnnotationChannel
libcorpusannotationchannel.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpusannotationchannel.i
	$(CPP) -c libcorpusannotationchannel_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpusannotationchannel_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpusannotationchannel.so

# AnotatedSentence
libcorpusannotatedsentence.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpusannotatedsentence.i
	$(CPP) -c libcorpusannotatedsentence_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpusannotatedsentence_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpusannotatedsentence.so

# AnnotationView
libcorpusannotationview.o:
	$(SWIG) $(SWIGOPTS_LANG) libcorpusannotationview.i
	$(CPP) -c libcorpusannotationview_wrap.cxx -I$(PYTHONDIR) $(CPPFLAGS)
	$(CPP) -shared libcorpusannotationview_wrap.o \
		$(PWRUTILBIN) $(CORPUS2BIN) -o _libcorpusannotationview.so

# -----------------------------------------------------------------------------
clean:
	rm -f $(CBIN) $(CBINOUT) $(CWRAP) $(CWRAPBIN) $(PYMODULES) $(PYCBIN)
