PROJECT(tagset-tool)

find_package(Libedit)
if (Libedit_FOUND)
	message(STATUS "Building with libedit")
	add_definitions( -DHAVE_LIBEDIT )
	set(LIBS ${LIBS} ${Libedit_LIBRARIES})
endif (Libedit_FOUND)

include_directories( ${CMAKE_SOURCE_DIR} )

add_executable( tagset-tool tagset-tool.cpp )
target_link_libraries ( tagset-tool corpus2 pwrutils ${Boost_LIBRARIES} ${LIBS})

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

if(UNIX)

	install(TARGETS tagset-tool
		RUNTIME DESTINATION bin)
	install(FILES corpus-get
		DESTINATION bin
		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif(UNIX)
