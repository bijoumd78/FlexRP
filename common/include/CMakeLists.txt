set(SRC
    flexrpsharedmemory.cpp
    flexrpsharedmemory.h
    ThreadPool.h
    zhelpers.hpp
    )

add_library(FlexSharedMemory ${SRC})
target_include_directories(FlexSharedMemory PRIVATE ${Boost_INCLUDE_DIRS})
if(UNIX)
    target_link_libraries(FlexSharedMemory PRIVATE ${Boost_LIBRARIES} pthread rt)
else()
    target_link_libraries(FlexSharedMemory PRIVATE ${Boost_LIBRARIES})
endif()

set_target_properties(FlexSharedMemory PROPERTIES FOLDER "include")

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
