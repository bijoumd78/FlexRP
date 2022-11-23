set(SRC
    Base_Modules.h
    Base_Modules.cpp
    )

add_library(Base_Modules ${SRC})
target_include_directories(Base_Modules PUBLIC ${ZeroMQ_INCLUDE_DIRS})
target_link_libraries(Base_Modules PUBLIC ${ZeroMQ_LIBRARIES} FLogger)

set_target_properties(Base_Modules PROPERTIES FOLDER "core")

install(TARGETS
    Base_Modules
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
