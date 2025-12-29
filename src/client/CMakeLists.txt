

Banner()

Headers(
	krunchercore
	krunchercore::tracer
	exexml
	datakruncher
	informationkruncher
	informationkruncher::client
	kruncher::krbuilder/
)


LibraryPaths(
	krunchercore::global/tracer/
	krunchercore::global/
	krunchercore::ut/Directory/
	krunchercore::ut/
	datakruncher::data/
	informationkruncher::socks/
	informationkruncher::client/
	kruncher::krbuilder/
)


Libraries(
	krunchertrace
	krunchercore
	datafactory
	informationkruncher
	infoclient
	krbuilder
)



add_executable(
	krbuild
		main.cpp
)



target_link_directories(
	krbuild
		PUBLIC
			${LIBRARY_PATHS}
)

target_include_directories(
    krbuild
	    PRIVATE
		../include/
		/home/jmt/libdb/build_unix/
		${INCLUDE_PATHS}
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)

target_link_libraries(
    krbuild
	    PUBLIC
		${LIBRARIES}
		ssl
		crypto
		/home/jmt/libdb/build_unix/libdb_cxx.a
)

install(
    TARGETS
	    krbuild
		    EXPORT exports
		    RUNTIME DESTINATION bin
) 


