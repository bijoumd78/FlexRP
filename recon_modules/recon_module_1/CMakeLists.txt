include_directories(../core)

set( SRC
    worker1.h
    worker1.cpp
    )

add_library(worker1_lib ${SRC})
target_link_libraries(worker1_lib PUBLIC ISMRMRD::ISMRMRD Base_Modules spdlog::spdlog)
set_target_properties(worker1_lib PROPERTIES FOLDER "recon_module_1")

add_executable(worker1 main.cpp)
target_link_libraries(worker1 PUBLIC worker1_lib)
set_target_properties(worker1 PROPERTIES FOLDER "recon_module_1")

#install
install (TARGETS
    worker1_lib
    worker1
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
