set( SOURCES
    logger.h
    configuration.h
    logger.cpp
    configuration.cpp
    )

add_library(FLogger ${SOURCES})
target_link_libraries(FLogger PUBLIC spdlog::spdlog)
set_target_properties(FLogger PROPERTIES FOLDER "logger")

#install
install (TARGETS
    FLogger
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