########## libcorpus2 ###############
PROJECT(corpus2)

set(corpus2_ver_major "0")
set(corpus2_ver_minor "0")
set(corpus2_ver_patch "1")


if(NOT LIBCORPUS2_SRC_DATA_DIR)
	message(FATAL_ERROR "There is no data for libcorpus2")
endif(NOT LIBCORPUS2_SRC_DATA_DIR)

set(LIBCORPUS2_PATH_SEPARATOR ";")
set(LIBCORPUS2_DATA_DIR
	".;${CMAKE_INSTALL_PREFIX}/${LIBCORPUS2_INSTALL_DATA_DIR}")
set(LIBCORPUS2_VERSION
	"${corpus2_ver_major}.${corpus2_ver_minor}.${corpus2_ver_patch}")

configure_file(config_d.in include/libcorpus2/config_d.h)
configure_file(version.in include/libcorpus2/version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/)
add_definitions(-DHAVE_CONFIG_D_H)
add_definitions(-DHAVE_VERSION_H)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/)

find_package(ICU REQUIRED QUIET)
include_directories(${ICU_INCLUDE_DIR})
link_directories(${ICU_LIBRARY_DIRS})
set(LIBS ${LIBS} icuuc icuio)

find_package(Loki REQUIRED QUIET)
set(LIBS ${LIBS} loki)

find_package(LibXML++ REQUIRED QUIET)
include_directories(${LibXML++_INCLUDE_DIRS})
link_directories(${LibXML++_LIBRARY_DIRS})
set(LIBS ${LIBS} ${LibXML++_LIBRARIES})

link_directories(${Boost_LIBRARY_DIRS})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

SET(libcorpus2_STAT_SRC
	chunk.cpp
	exception.cpp
	lexeme.cpp
	sentence.cpp
	tag.cpp
	tagset.cpp
	tagsetmanager.cpp
	tagsetparser.cpp
	token.cpp
	io/orthwriter.cpp
	io/plainwriter.cpp
	io/reader.cpp
	io/rft.cpp
	io/sax.cpp
	io/writer.cpp
	io/xces.cpp
	io/xcescommon.cpp
	io/xcesreader.cpp
	io/xcesvalidate.cpp
	io/xceswriter.cpp
	util/settings.cpp
	util/symboldictionary.cpp
	util/tokentimer.cpp
)

file(GLOB_RECURSE INCS "*.h")

add_library(corpus2 SHARED ${libcorpus2_STAT_SRC} ${INCS})
target_link_libraries(corpus2 ${LIBS})
set_target_properties(corpus2 PROPERTIES
	VERSION "${corpus2_ver_major}.${corpus2_ver_minor}"
	SOVERSION ${corpus2_ver_major})

if(UNIX)
	install(TARGETS corpus2
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

