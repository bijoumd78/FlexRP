include_directories(../core)

set( SRC
    worker3.h
    worker3.cpp
    )

add_library(worker3_lib ${SRC})
target_link_libraries(worker3_lib PUBLIC ISMRMRD::ISMRMRD Base_Modules spdlog::spdlog FlexSharedMemory )
set_target_properties(worker3_lib PROPERTIES FOLDER "recon_module_3")

add_executable(worker3 main.cpp)
target_link_libraries(worker3 PUBLIC worker3_lib)
set_target_properties(worker3 PROPERTIES FOLDER "recon_module_3")

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
