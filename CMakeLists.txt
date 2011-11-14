PROJECT(Corpus2Library)

set(corpus2_ver_major "1")
set(corpus2_ver_minor "0")
set(corpus2_ver_patch "14")

cmake_minimum_required(VERSION 2.8.0)

# use some of our own Find* scripts
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeScripts)

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CORPUS2_BUILD_SWIG True CACHE BOOL "Build SWIG Python wrappers")
set(CORPUS2_BUILD_POLIQARP True CACHE BOOL "Build Poliqarp library and wraper")

if(CMAKE_COMPILER_IS_GNUCXX)
	# Set our own default flags at first run.
	if(NOT CONFIGURED)

		if(ENABLE_STRICT_COMPILATION)
			set(STRICT_FLAGS "-Werror -Wno-unused-parameter")
		else(ENABLE_STRICT_COMPILATION)
			set(STRICT_FLAGS "")
		endif(ENABLE_STRICT_COMPILATION)


		# Strict compilation for C files is disabled until somebody wants to clean them.
		set(CMAKE_C_FLAGS "-W -Wall -ansi $ENV{CFLAGS}" 
				CACHE STRING "Flags used by the C compiler during normal builds." FORCE)
		set(CMAKE_C_FLAGS_DEBUG "-O0 -DDEBUG -ggdb3 -W -Wall -ansi $ENV{CFLAGS}"
				CACHE STRING "Flags used by the C compiler during debug builds." FORCE)

		set(CMAKE_CXX_FLAGS "-W -Wall -ansi ${STRICT_FLAGS} $ENV{CXXFLAGS}"
				CACHE STRING "Flags used by the CXX compiler during normal builds." FORCE)
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DDEBUG -ggdb3 -W -Wall -ansi ${STRICT_FLAGS} $ENV{CXXFLAGS}"
				CACHE STRING "Flags used by the CXX compiler during debug builds." FORCE)

		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -fno-omit-frame-pointer" 
				CACHE STRING "Flags used by the CXX compiler during relwithdebiginfo builds" FORCE)


	endif(NOT CONFIGURED)

endif(CMAKE_COMPILER_IS_GNUCXX)

set(LIBS "")
include_directories(${Corpus2Library_SOURCE_DIR})

find_package(Boost 1.41 REQUIRED COMPONENTS program_options system filesystem regex)

if(MSVC OR BORLAND)
# Use the auto-linking feature, don't try to add libraries yourself:
	set(Boost_LIBRARIES "")
# Note the default is to link static libs and it is left like that here.
# You can make it link to DLL by defining BOOST_#module#_DYN_LINK
# in C++ source file or somewhere in CMake file, e.g. 
# add_definitions( -DBOOST_FILESYSTEM_DYN_LINK )
endif(MSVC OR BORLAND)

set(LIBCORPUS2_INSTALL_DATA_DIR share/corpus2)
FIND_PATH(LIBCORPUS2_SRC_DATA_DIR
	kipi.tagset
	${CMAKE_SOURCE_DIR}/corpus2data
)
MARK_AS_ADVANCED(LIBCORPUS2_SRC_DATA_DIR)

add_subdirectory(libpwrutils)
add_subdirectory(libcorpus2)
add_subdirectory(corpus2tools)
add_subdirectory(tests)
if(CORPUS2_BUILD_POLIQARP)
	add_subdirectory(poliqarp-library)
	add_subdirectory(poliqarp)
else()
	message(STATUS "Not building Poliqarp library and wrapper")
endif(CORPUS2_BUILD_POLIQARP)

if(CORPUS2_BUILD_SWIG)
	FIND_PACKAGE(SWIG)
	if(SWIG_FOUND)
		message(STATUS "SWIG found, Python wrappers will be built")
		add_subdirectory(swig)
	else()
		message(STATUS "WARNING: SWIG not found, Python wrappers will not be built")
	endif(SWIG_FOUND)
else()
	message(STATUS "Not building SWIG Python wrappers")
endif(CORPUS2_BUILD_SWIG)

