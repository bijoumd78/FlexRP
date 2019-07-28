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
         worker1 DESTINATION ${CMAKE_SOURCE_DIR}/bin)
