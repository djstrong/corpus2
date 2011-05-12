PROJECT( test )

include_directories( ${CMAKE_SOURCE_DIR} )

add_definitions(-DLIBWCCL_TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/")

add_executable( tests
	main.cpp
	ann_basic.cpp
	basic.cpp
	io.cpp
	ioann.cpp
	tag_split.cpp
	tagset_parse.cpp
	tokenmetadata.cpp
)

target_link_libraries ( tests corpus2 pwrutils ${Boost_LIBRARIES})

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

add_custom_target(test tests)
