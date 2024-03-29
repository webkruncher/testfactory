cmake_minimum_required(VERSION 3.10)

project("AutoMake2Cmake")


message( SYSTEM: ${CMAKE_SYSTEM_NAME} ) 

add_compile_options(-ggdb)

set(
	NIX_INCLUDE_PATHS
		/usr/local/include/infokruncher/
		../include
)

set(
	NIX_LIBRARY_PATHS
		/usr/local/lib/infokruncher/
)

set(
	NIX_LIBRARIES
			informationkruncher
)

IF(CMAKE_SYSTEM_NAME STREQUAL OpenBSD)
	add_definitions( -DUNIX )
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
	add_definitions( -DLINUX )
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
	)
ENDIF()


add_subdirectory( src )


