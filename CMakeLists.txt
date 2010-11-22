PROJECT(Corpus2Library)
cmake_minimum_required(VERSION 2.8.0)

# use some of our own Find* scripts
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeScripts)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release
RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)


if(CMAKE_COMPILER_IS_GNUCXX)
	# Set our own default flags at first run.
	if(NOT CONFIGURED)

		if(ENABLE_STRICT_COMPILATION)
			set(STRICT_FLAGS "-Werror -Wno-unused-parameter")
		else(ENABLE_STRICT_COMPILATION)
			set(STRICT_FLAGS "")
		endif(ENABLE_STRICT_COMPILATION)


		# Strict compilation for C files is disabled until somebody wants to clean them.
		set(CMAKE_C_FLAGS"-W -Wall -ansi $ENV{CFLAGS}" 
				CACHE STRING "Flags used by the C compiler during normal builds." FORCE)
		set(CMAKE_C_FLAGS_DEBUG "-O0 -DDEBUG -ggdb3 -W -Wall -ansi $ENV{CFLAGS}"
				CACHE STRING "Flags used by the C compiler during debug builds." FORCE)

		set(CMAKE_CXX_FLAGS "-W -Wall -ansi ${STRICT_FLAGS} $ENV{CXXFLAGS}"
				CACHE STRING "Flags used by the CXX compiler during normal builds." FORCE)
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DDEBUG -ggdb3 -W -Wall -ansi ${STRICT_FLAGS} $ENV{CXXFLAGS}"
				CACHE STRING "Flags used by the CXX compiler during debug builds." FORCE)

	endif(NOT CONFIGURED)

endif(CMAKE_COMPILER_IS_GNUCXX)

set(LIBS "")
include_directories(${Corpus2Library_SOURCE_DIR})

set(Boost_USE_STATIC_LIBS        OFF)
find_package(Boost 1.41 REQUIRED COMPONENTS program_options filesystem regex)

set(LIBCORPUS2_INSTALL_DATA_DIR share/corpus2)
FIND_PATH(LIBCORPUS2_SRC_DATA_DIR
    test.tagset
    ${CMAKE_SOURCE_DIR}/corpus2data
)
MARK_AS_ADVANCED(LIBCORPUS2_SRC_DATA_DIR)

add_subdirectory(libpwrutils)
add_subdirectory(libcorpus2)
add_subdirectory(tagset-tool)
add_subdirectory(tests)

