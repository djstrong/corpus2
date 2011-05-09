########## corpus2 wrappers -- SWIG ###############

PROJECT(corpus2SwigWrap)

find_package(Corpus2 1.0.8 REQUIRED)
set(CORPUS2_LIBS ${Corpus2_LIBRARY})

find_package(PwrUtils 1.0.1 REQUIRED)
set(PWRUTILS_LIBS ${PwrUtils_LIBRARY})
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
# libpwrnlperror
SET_SOURCE_FILES_PROPERTIES(libpwrnlperror.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libpwrnlperror.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libpwrnlperror python libpwrnlperror.i )
SWIG_LINK_LIBRARIES(libpwrnlperror ${PYTHON_LIBRARIES} ${PWRUTILS_LIBS})

# -----------------------------------------------------------------------------
# libcorpus2exception
SET_SOURCE_FILES_PROPERTIES(libcorpus2exception.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpus2exception.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpus2exception python libcorpus2exception.i )
SWIG_LINK_LIBRARIES(libcorpus2exception ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpustag
SET_SOURCE_FILES_PROPERTIES(libcorpustag.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpustag.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpustag python libcorpustag.i )
SWIG_LINK_LIBRARIES(libcorpustag ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpustagset
SET_SOURCE_FILES_PROPERTIES(libcorpustagset.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpustagset.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpustagset python libcorpustagset.i )
SWIG_LINK_LIBRARIES(libcorpustagset ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpustagsetmanager
SET_SOURCE_FILES_PROPERTIES(libcorpustagsetmanager.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpustagsetmanager.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpustagsetmanager python libcorpustagsetmanager.i )
SWIG_LINK_LIBRARIES(libcorpustagsetmanager ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpuslexeme
SET_SOURCE_FILES_PROPERTIES(libcorpuslexeme.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpuslexeme.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpuslexeme python libcorpuslexeme.i )
SWIG_LINK_LIBRARIES(libcorpuslexeme ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpussentence
SET_SOURCE_FILES_PROPERTIES(libcorpussentence.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpussentence.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpussentence python libcorpussentence.i )
SWIG_LINK_LIBRARIES(libcorpussentence ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpuschunk
SET_SOURCE_FILES_PROPERTIES(libcorpuschunk.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpuschunk.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpuschunk python libcorpuschunk.i )
SWIG_LINK_LIBRARIES(libcorpuschunk ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpustoken
SET_SOURCE_FILES_PROPERTIES(libcorpustoken.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpustoken.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpustoken python libcorpustoken.i )
SWIG_LINK_LIBRARIES(libcorpustoken ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpustokenwriter
SET_SOURCE_FILES_PROPERTIES(libcorpustokenwriter.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpustokenwriter.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpustokenwriter python libcorpustokenwriter.i )
SWIG_LINK_LIBRARIES(libcorpustokenwriter ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpustokenwriter
SET_SOURCE_FILES_PROPERTIES(libcorpustokenreader.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpustokenreader.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpustokenreader python libcorpustokenreader.i )
SWIG_LINK_LIBRARIES(libcorpustokenreader ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpusiob
SET_SOURCE_FILES_PROPERTIES(libcorpusiob.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpusiob.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpusiob python libcorpusiob.i )
SWIG_LINK_LIBRARIES(libcorpusiob ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpusannotationchannel
SET_SOURCE_FILES_PROPERTIES(libcorpusannotationchannel.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpusannotationchannel.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpusannotationchannel python libcorpusannotationchannel.i )
SWIG_LINK_LIBRARIES(libcorpusannotationchannel ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpusannotationchannel
SET_SOURCE_FILES_PROPERTIES(libcorpusannotatedsentence.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpusannotatedsentence.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpusannotatedsentence python libcorpusannotatedsentence.i )
SWIG_LINK_LIBRARIES(libcorpusannotatedsentence ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# libcorpusannotationchannel
SET_SOURCE_FILES_PROPERTIES(libcorpusannotationview.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libcorpusannotationview.i PROPERTIES SWIG_FLAGS "-includeall" )
SWIG_ADD_MODULE(libcorpusannotationview python libcorpusannotationview.i )
SWIG_LINK_LIBRARIES(libcorpusannotationview ${PYTHON_LIBRARIES} ${CORPUS2_PWR_LIBS})

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

set_target_properties(
	_libpwrnlperror
	_libcorpus2exception
	_libcorpustag
	_libcorpustagset
	_libcorpustagsetmanager
	_libcorpuslexeme
	_libcorpustoken
	_libcorpussentence
	_libcorpuschunk
	_libcorpustokenwriter
	_libcorpustokenreader
	_libcorpusiob
	_libcorpusannotationchannel
	_libcorpusannotatedsentence
	_libcorpusannotationview
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
	TARGETS _libpwrnlperror _libcorpus2exception _libcorpustag _libcorpustagset
		_libcorpustagsetmanager _libcorpuslexeme _libcorpustoken
		_libcorpussentence _libcorpuschunk _libcorpustokenwriter 
		_libcorpustokenreader _libcorpusiob _libcorpusannotationchannel 
		_libcorpusannotatedsentence _libcorpusannotationview

	LIBRARY
	DESTINATION ${PYTHON_INSTDIR}
	PERMISSIONS ${PERM_SCRIPTS}
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/libpwrnlperror.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpus2exception.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpus2exception.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpustag.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpustagset.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpustagsetmanager.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpuslexeme.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpustoken.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpussentence.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpuschunk.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpustokenwriter.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpustokenreader.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpusiob.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpusannotationchannel.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpusannotatedsentence.py
	${CMAKE_CURRENT_BINARY_DIR}/libcorpusannotationview.py
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
	DESTINATION ${SWIG_SRC_DIR}
	PERMISSIONS ${PERM_SCRIPTS}
)
