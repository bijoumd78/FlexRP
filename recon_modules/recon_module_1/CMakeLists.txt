include_directories(../core)

set( SRC
    worker1.h
    worker1.cpp
    )

add_library(worker1_lib ${SRC})
target_link_libraries(worker1_lib PRIVATE Base_Modules spdlog::spdlog)

add_executable(worker1 main.cpp)
target_link_libraries(worker1 PRIVATE worker1_lib)

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
