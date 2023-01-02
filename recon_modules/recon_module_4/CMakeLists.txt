include_directories(../core)

set( SRC
    worker4.h
    worker4.cpp
    )

add_library(worker4_lib ${SRC})
target_link_libraries(worker4_lib PUBLIC FLogger ISMRMRD::ISMRMRD Base_Modules)
set_target_properties(worker4_lib PROPERTIES FOLDER "recon_modules")

add_executable(consumer main.cpp)
target_link_libraries(consumer PUBLIC worker4_lib)
set_target_properties(consumer PROPERTIES FOLDER "recon_modules")

#install
install (TARGETS
    worker4_lib
    consumer
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
