include_directories(../core)

set( SRC
    worker6.h
    worker6.cpp
    )

add_library(worker6_lib ${SRC})
target_link_libraries(worker6_lib PUBLIC ISMRMRD::ISMRMRD Base_Modules spdlog::spdlog FlexSharedMemory )

add_executable(worker6 main.cpp)
target_link_libraries(worker6 PUBLIC worker6_lib)

#install
install (TARGETS
    worker6_lib
    worker6
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
