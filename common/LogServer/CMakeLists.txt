set( SRC
    configuration.h
    configuration.cpp
    fileUtility.h
    fileUtility.cpp
    main.cpp
    )

add_executable(LogServer ${SRC})
set_target_properties(LogServer PROPERTIES FOLDER "LogServer")

#install
install (TARGETS
    LogServer
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

message (STATUS "Copying logging configuration file: logConfig.json to the binary directory")
add_custom_command(TARGET LogServer 
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                    ${PROJECT_SOURCE_DIR}/logConfigFiles/logConfig.json 
                    $<TARGET_FILE_DIR:LogServer>)