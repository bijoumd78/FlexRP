set(SRC
    flexrpsharedmemory.h
    flexrpsharedmemory.cpp
    )

add_library(FlexSharedMemory ${SRC})
target_link_libraries(FlexSharedMemory ${Boost_LIBRARIES} pthread rt)

