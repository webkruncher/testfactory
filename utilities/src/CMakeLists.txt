
cmake_minimum_required(VERSION 3.10)

project("testfactoryutilities")

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../krunchercore/cmake/CMakeLists.txt)


file(READ ${CMAKE_CURRENT_LIST_DIR}/Utilities.txt utilities)
STRING(REGEX REPLACE "\n" ";" utilities "${utilities}")


foreach( Target ${utilities} )
	install(
	    PROGRAMS
		    ${Target}
			    DESTINATION bin
	) 

endforeach()

