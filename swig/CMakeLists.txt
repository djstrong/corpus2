########## corpus2 wrappers -- SWIG ###############

PROJECT(corpus2SwigWrap)

set(CORPUS2_LIBS corpus2)
set(PWRUTILS_LIBS pwrutils)
set(CORPUS2_PWR_LIBS ${CORPUS2_LIBS} ${PWRUTILS_LIBS})

include_directories (${corpus2_SOURCE_DIR})
include_directories (${pwrutils_SOURCE_DIR})

link_directories(${Libcorpus2_BINARY_DIR})

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

find_package(PythonLibs)
find_package(PythonInterp)
# idea taken from pyplot build system 
execute_process(
	COMMAND
	${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print sysconfig.get_python_lib(1,0,prefix='${CMAKE_INSTALL_EXEC_PREFIX}')"
	OUTPUT_VARIABLE PYTHON_INSTDIR
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "INFO: " "python lib: ${PYTHON_INSTDIR}" )

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-ansi $ENV{CXXFLAGS}")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DDEBUG -ggdb3 -ansi $ENV{CXXFLAGS}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -fno-omit-frame-pointer")
endif(CMAKE_COMPILER_IS_GNUCXX)

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

SET(CMAKE_SWIG_FLAGS "")
SET(SWIG_SRC_DIR ${SWIG_DIR}/libcorpus2)

# -----------------------------------------------------------------------------
# corous2
SET_SOURCE_FILES_PROPERTIES(corpus2.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(corpus2.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(corpus2 python corpus2.i )
SWIG_LINK_LIBRARIES(corpus2 ${PYTHON_LIBRARIES} ${PWRUTILS_LIBS})
SWIG_LINK_LIBRARIES(corpus2 ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

set_target_properties(
	_corpus2
	PROPERTIES
	INSTALL_NAME_DIR "${PYTHON_INSTDIR}"
)

set(PERM_SCRIPTS
	OWNER_READ
	OWNER_WRITE
	OWNER_EXECUTE
	GROUP_READ
	GROUP_EXECUTE
	WORLD_READ
	WORLD_EXECUTE
)

# -----------------------------------------------------------------------------
# Install python modules
# -----------------------------------------------------------------------------

install(
	TARGETS _corpus2 
	LIBRARY
	DESTINATION ${PYTHON_INSTDIR}
	PERMISSIONS ${PERM_SCRIPTS}
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/corpus2.py
	DESTINATION ${PYTHON_INSTDIR}
	PERMISSIONS ${PERM_SCRIPTS}
)

# -----------------------------------------------------------------------------
# Install SWIG files
# -----------------------------------------------------------------------------

install(
	FILES libpwrnlperror.i
	libcorpus2exception.i
	libcorpus2exception.i
	libcorpustag.i
	libcorpustagset.i
	libcorpustagsetmanager.i
	libcorpuslexeme.i
	libcorpustoken.i
	libcorpussentence.i
	libcorpuschunk.i
	libcorpustokenwriter.i
	libcorpustokenreader.i
	libcorpusiob.i
	libcorpusannotationchannel.i
	libcorpusannotatedsentence.i
	libcorpusannotationview.i
	corpus2.i
	boost_shared_ptr.i
	unicodestring.i
	DESTINATION ${SWIG_SRC_DIR}
	PERMISSIONS ${PERM_SCRIPTS}
)
