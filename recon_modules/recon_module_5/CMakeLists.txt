include_directories(../core)

set( SRC
    worker5.h
    worker5.cpp
    )

add_library(worker5_lib ${SRC})
target_link_libraries(worker5_lib PRIVATE Base_Modules spdlog::spdlog)

add_executable(consumer2 main.cpp)
target_link_libraries(consumer2 PRIVATE worker5_lib)

#install
install (TARGETS
    worker5_lib
    consumer2
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
