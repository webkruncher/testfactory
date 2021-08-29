
set(CMAKE_CXX_FLAGS " -std=c++11 -pthread " )

get_filename_component(INCLUDES_PATH ../include/ REALPATH) 

add_executable(
	restful
		main.cpp
)

#add_library( kruncher::restful  ALIAS restful  )

target_include_directories(
    restful
	    PRIVATE
		${INCLUDE_PATHS}
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)

target_link_directories(
	restful
		PUBLIC
			${LIBRARY_PATHS}
)

target_link_libraries(
    restful
	    PUBLIC
			${LIBRARIES}
)

install(
    TARGETS
	    restful
		    EXPORT exports
		    RUNTIME DESTINATION bin
) 

