Banner()
Headers(
	krunchercore
	krunchercore::global/tracer/
	datakruncher
	informationkruncher
	tradekruncher/db
#	librarykruncher
)

add_library(
	libinfo
		InfoBuildFace.cpp
		LibCatalog.cpp
		InfoLibData.cpp
)

add_library( infobuild::libinfo  ALIAS libinfo  )

target_include_directories(
    libinfo
	    PRIVATE
		../include/
		/home/jmt/libdb/build_unix/
		${INCLUDE_PATHS}
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)

target_link_directories(
	libinfo
		PUBLIC
			${LIBRARY_PATHS}
)



install(TARGETS libinfo DESTINATION lib/infokruncher/testfactory/db )

