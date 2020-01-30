set(SRC
    flexrpsharedmemory.h
    flexrpsharedmemory.cpp
    ThreadPool.h
    zhelpers.hpp
    )

add_library(FlexSharedMemory ${SRC})
target_link_libraries(FlexSharedMemory ${Boost_LIBRARIES} pthread rt)

install(
    TARGETS
    FlexSharedMemory
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
    COMPONENT dev
    )
