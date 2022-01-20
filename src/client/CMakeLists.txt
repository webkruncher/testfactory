
set(INCLUDE_PATHS)
IncludeProjects(
	krunchercore/src/include
	exexml/src/include
	informationkruncher/src/include
	informationkruncher/src/include/db
)

set(LIBRARY_PATHS)
set(LIBRARIES)
IncludeLibraryPaths(
	krunchercore
	informationkruncher
)


add_executable(
	restful
		main.cpp
		restful.cpp
)


target_include_directories(
    restful
	    PRIVATE
		../include/
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
		ssl
		crypto
		/home/jmt/libdb/build_unix/libdb_cxx.a
)

install(
    TARGETS
	    testfactory
		    EXPORT exports
		    RUNTIME DESTINATION bin
) 


