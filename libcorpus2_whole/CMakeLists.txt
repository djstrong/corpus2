PROJECT(corpus2_whole)

find_package(LibXML++ REQUIRED QUIET)
include_directories(${LibXML++_INCLUDE_DIRS})
link_directories(${LibXML++_LIBRARY_DIRS})
set(LIBS ${LIBS} ${LibXML++_LIBRARIES})

SET(libcorpus2_whole_SRC
	corpus.cpp
	document.cpp
	relation.cpp
	io/reader_i.h
	io/relreader.cpp
	io/cclrelreader.cpp
	io/documentcorpusreader.cpp
	io/corpusreader.cpp
	io/documentreader.cpp

)

if(CORPUS2_BUILD_POLIQARP)
	SET(libcorpus2_whole_SRC ${libcorpus2_whole_SRC}
		io/poliqarpdocumentreader.cpp
		io/poliqarpcorpusreader.cpp
	)
endif(CORPUS2_BUILD_POLIQARP)

file(GLOB_RECURSE INCS "*.h")

if(WIN32)
	add_library(corpus2_whole STATIC ${libcorpus2_whole_SRC} ${INCS})
else(WIN32)
	add_library(corpus2_whole SHARED ${libcorpus2_whole_SRC} ${INCS})
endif(WIN32)
target_link_libraries(corpus2_whole corpus2 ${LIBS})

if(UNIX)
	install(TARGETS corpus2_whole
		LIBRARY DESTINATION lib)

	install(
		DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		DESTINATION include
		FILES_MATCHING PATTERN "*.h"
		PATTERN ".svn" EXCLUDE
		PATTERN "bin" EXCLUDE
		PATTERN "build" EXCLUDE
		PATTERN "CMake*" EXCLUDE
	)
	install(
		DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/
		DESTINATION include
		FILES_MATCHING PATTERN "version.h"
		PATTERN "config_d.h"
	)
	install(
		DIRECTORY ${LIBCORPUS2_SRC_DATA_DIR}/
		DESTINATION ${LIBCORPUS2_INSTALL_DATA_DIR}
		FILES_MATCHING PATTERN "*.ini"
		PATTERN "*.tagset"
		PATTERN ".svn" EXCLUDE
		PATTERN "local/*" EXCLUDE
	)
endif(UNIX)

