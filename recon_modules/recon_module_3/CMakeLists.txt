include_directories(../core)

set( SRC
     worker3.h
     worker3.cpp
     )

add_library(worker3_lib ${SRC})
target_link_libraries(worker3_lib PRIVATE Base_Modules spdlog::spdlog FlexSharedMemory)

add_executable(worker3 main.cpp)
target_link_libraries(worker3 PRIVATE worker3_lib)

#install
install (TARGETS
         worker3 DESTINATION ${CMAKE_SOURCE_DIR}/bin)
