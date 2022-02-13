
Banner()


Headers(
	krunchercore
	krunchercore::tracer
	exexml
	informationkruncher
	informationkruncher::socks/
)


LibraryPaths(
	krunchercore::global/tracer/
	krunchercore::global/
)


Libraries(
	krunchertrace
	krunchercore
)

add_library(
	krbuilder
		STATIC
			krbuilder.cpp
			buildservice.cpp
)

add_library( kruncher::krbuilder  ALIAS krbuilder  )

target_include_directories(
    krbuilder
	    PRIVATE
		../include/
		${INCLUDE_PATHS}
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)

target_link_libraries(
    krbuilder
	    PUBLIC
)

	    
install(TARGETS krbuilder DESTINATION lib/infokruncher/testfactory/krbuilder )

