include_directories(../core)

set( SRC
     worker2.h
     worker2.cpp
     )

add_library(worker2_lib ${SRC})
target_link_libraries(worker2_lib PRIVATE Base_Modules spdlog::spdlog FlexSharedMemory)

add_executable(worker2 main.cpp)
target_link_libraries(worker2 PRIVATE worker2_lib)

#install
install (TARGETS
         worker2 DESTINATION ${CMAKE_SOURCE_DIR}/bin)
