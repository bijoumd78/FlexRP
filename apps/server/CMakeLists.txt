include_directories(../logger)

set( SOURCES
    Flexrp_xml.h
    Flexrp_xml.cpp
    )

add_library(Flexrp_xml ${SOURCES})
target_link_libraries(Flexrp_xml PUBLIC ${Boost_LIBRARIES} pugixml FLogger FlexSharedMemory)
set_target_properties(Flexrp_xml PROPERTIES FOLDER "server")

add_executable(FleXRP main.cpp)
target_link_libraries(FleXRP PUBLIC Flexrp_xml)
set_target_properties(FleXRP PROPERTIES FOLDER "server")

#install
install (TARGETS
    Flexrp_xml
    FleXRP
    ARCHIVE
    DESTINATION ${INSTALL_LIBDIR}
    COMPONENT lib
    RUNTIME
    DESTINATION ${INSTALL_BINDIR}
    COMPONENT bin
    LIBRARY
    DESTINATION ${INSTALL_LIBDIR}
    COMPONENT lib
    PUBLIC_HEADER
    DESTINATION ${INSTALL_INCLUDEDIR}
    COMPONENT dev)

message (STATUS "Copying logging configuration file: Config.json to the binary directory")
add_custom_command(TARGET FleXRP 
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                    ${PROJECT_SOURCE_DIR}/logConfigFiles/logConfig.JSON 
                    $<TARGET_FILE_DIR:FleXRP>)