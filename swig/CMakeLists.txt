########## corpus2 wrappers -- SWIG ###############

PROJECT(corpus2SwigWrap)

#find_package(Corpus2 1.0.8 REQUIRED)
set(CORPUS2_LIBS corpus2)

#find_package(PwrUtils 1.0.1 REQUIRED)
set(PWRUTILS_LIBS pwrutils)
set(CORPUS2_PWR_LIBS ${CORPUS2_LIBS} ${PWRUTILS_LIBS})

include_directories (${Libcorpus2_SOURCE_DIR} "../libcorpus2")
include_directories (${Libpwrutils_SOURCE_DIR} "../libpwrutils")

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

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

SET(CMAKE_SWIG_FLAGS "")
SET(SWIG_SRC_DIR ${SWIG_DIR}/libcorpus)

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
	DESTINATION ${SWIG_SRC_DIR}
	PERMISSIONS ${PERM_SCRIPTS}
)
