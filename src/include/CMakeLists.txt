


Banner()


file(GLOB includes ${CMAKE_CURRENT_LIST_DIR}/*.h)

install(
    FILES
            ${includes}    
                    OPTIONAL
                    DESTINATION include/infokruncher/testfactory/
)

add_library(
    krunchbuildheaders
            INTERFACE
)

add_library( infobuild::krunchbuildheaders ALIAS krunchbuildheaders )

target_include_directories(
    krunchbuildheaders
            INTERFACE
                    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
                    $<INSTALL_INTERFACE:include>
			../include
)

target_link_libraries(
    krunchbuildheaders
            INTERFACE
)




