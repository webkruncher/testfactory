cmake_minimum_required(VERSION 3.10)

project("restful")

#find_library(infokruncher REQUIRED )

message( SYSTEM: ${CMAKE_SYSTEM_NAME} ) 

set(
	NIX_INCLUDE_PATHS
		/usr/local/include/infokruncher/
		/usr/include/uuid/
		../include
)

set(
	NIX_LIBRARY_PATHS
		/usr/local/lib/infokruncher/
)

set(
	NIX_LIBRARIES
			informationkruncher
			ssl	
			crypto	
)

IF(CMAKE_SYSTEM_NAME STREQUAL OpenBSD)
	add_definitions( -DUNIX -Wall )
	set(
		INCLUDE_PATHS
			${NIX_INCLUDE_PATHS}
	)
	set(
		LIBRARY_PATHS
			${NIX_LIBRARY_PATHS}
	)
	set(
		LIBRARIES
			${NIX_LIBRARIES}
	)
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL Linux)
	add_definitions( -DLINUX -Wall )
	set(
		INCLUDE_PATHS
			${NIX_INCLUDE_PATHS}
	)
	set(
		LIBRARY_PATHS
			${NIX_LIBRARY_PATHS}
			/usr/lib/x86_64-linux-gnu/
	)
	set(
		LIBRARIES
			${NIX_LIBRARIES}
			uuid
	)
ENDIF()


add_subdirectory( include )
add_subdirectory( client )


