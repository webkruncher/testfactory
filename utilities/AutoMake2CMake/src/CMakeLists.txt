
set(CMAKE_CXX_FLAGS " -std=c++11 -pthread " )

get_filename_component(INCLUDES_PATH ../include/ REALPATH) 

add_executable(
	CreateCmakeMacros 
		CreateCmakeMacros.cpp
)


target_include_directories(
    CreateCmakeMacros 
	    PRIVATE
		${INCLUDE_PATHS}
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)

target_link_directories(
	CreateCmakeMacros 
		PUBLIC
			${LIBRARY_PATHS}
)

target_link_libraries(
    CreateCmakeMacros 
	    PUBLIC
			${LIBRARIES}
)

install(
    TARGETS
	    CreateCmakeMacros 
		    EXPORT exports
		    RUNTIME DESTINATION bin
) 

