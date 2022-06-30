include_directories(../core)

set( SRC
    worker7.h
    worker7.cpp
    )

add_library(worker7_lib ${SRC})
target_link_libraries(worker7_lib PUBLIC ISMRMRD::ISMRMRD Base_Modules spdlog::spdlog FlexSharedMemory )

add_executable(worker7 main.cpp)
target_link_libraries(worker7 PUBLIC worker7_lib)

#install
install (TARGETS
    worker7_lib
    worker7
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
