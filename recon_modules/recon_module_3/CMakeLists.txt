include_directories(../core)

set( SRC
    worker3.h
    worker3.cpp
    )

add_library(worker3_lib ${SRC})
target_link_libraries(worker3_lib PUBLIC ISMRMRD::ISMRMRD Base_Modules spdlog::spdlog FlexSharedMemory )

add_executable(worker3 main.cpp)
target_link_libraries(worker3 PUBLIC worker3_lib)

#install
install (TARGETS
    worker3_lib
    worker3
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
