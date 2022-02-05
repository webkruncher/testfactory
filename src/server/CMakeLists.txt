
Banner()


Headers(
	krunchercore
	krunchercore::tracer
	exexml
	datakruncher
	informationkruncher
	infodata
	infodata::db
	infodata::db/auth
	infodata::db/site
)


LibraryPaths(
	krunchercore::global/tracer/
	krunchercore::global/
	krunchercore::ut/Directory/
	krunchercore::ut/
	datakruncher::data/
	informationkruncher::socks/
	infodata::db/auth/
	infodata::db/site/
	datakruncher::data/
	kruncher::krbuilder/
)


Libraries(
	krunchertrace
	krunchercore
	datafactory
	informationkruncher
	auth
	site
	krbuilder
)


add_executable(
	infobuilder
		main.cpp
		buildservice.cpp
)


target_include_directories(
    infobuilder
	    PRIVATE
		${INCLUDE_PATHS}
		../include/
		/home/jmt/libdb/build_unix
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)

target_link_directories(
	infobuilder
		PUBLIC
			${LIBRARY_PATHS}
)

target_link_libraries(
    infobuilder
	    PUBLIC
		ssl
		crypto
		/home/jmt/libdb/build_unix/libdb_cxx.a
		${LIBRARIES}
)

install(
    TARGETS
	    infobuilder
		    EXPORT exports
		    RUNTIME DESTINATION bin
) 

