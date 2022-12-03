set(SRC
    logger.h
	logger.cpp
    )

add_library(FLogger ${SRC})
target_include_directories(FLogger PRIVATE ${Boost_INCLUDE_DIRS})
if(UNIX)
    target_link_libraries(FLogger PRIVATE ${Boost_LIBRARIES} pthread rt)
else()
    target_link_libraries(FLogger PRIVATE ${Boost_LIBRARIES})
endif()

set_target_properties(FLogger PROPERTIES FOLDER "LogClient")

install(
    TARGETS
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
    COMPONENT dev
    )
