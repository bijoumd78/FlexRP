include_directories(../core)

set( SRC
    worker2.h
    worker2.cpp
    )

add_library(worker2_lib ${SRC})
target_link_libraries(worker2_lib PUBLIC FLogger ISMRMRD::ISMRMRD Base_Modules FlexSharedMemory )
set_target_properties(worker2_lib PROPERTIES FOLDER "recon_modules")

add_executable(worker2 main.cpp)
target_link_libraries(worker2 PUBLIC worker2_lib)
set_target_properties(worker2 PROPERTIES FOLDER "recon_modules")

#install
install (TARGETS
    worker2_lib
    worker2
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
