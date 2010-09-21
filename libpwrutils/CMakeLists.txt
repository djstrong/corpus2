########## libpwrutils ###############

PROJECT(pwrutils)

set(pwrutils_ver_major "0")
set(pwrutils_ver_minor "0")
set(pwrutils_ver_patch "1")

set(LIBPWRUTILS_VERSION
	"${pwrutils_ver_major}.${pwrutils_ver_minor}.${pwrutils_ver_patch}")


configure_file(version.in include/libpwrutils/version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/)
add_definitions(-DHAVE_VERSION_H)
set(LIBS "")

include_directories( ${CMAKE_SOURCE_DIR} )

find_package(ICU REQUIRED)
include_directories(${ICU_INCLUDE_DIR})
link_directories(${ICU_LIBRARY_DIRS})
set(LIBS ${LIBS} icuuc icuio)

link_directories(${Boost_LIBRARY_DIRS})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

SET(libpwrutils_STAT_SRC
	exception.cpp
	whitespace.cpp
	pathsearch.cpp
	util.cpp
)

file(GLOB_RECURSE INCS "*.h")

add_library(pwrutils SHARED ${libpwrutils_STAT_SRC} ${INCS})
target_link_libraries(pwrutils ${LIBS} )
set_target_properties(pwrutils PROPERTIES
	VERSION "${pwrutils_ver_major}.${pwrutils_ver_minor}"
	SOVERSION ${pwrutils_ver_major})

if(UNIX)
	install(TARGETS pwrutils
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
	)
endif(UNIX)

