include_directories(../core)

set( SRC
     worker4.h
     worker4.cpp
     )

add_library(worker4_lib ${SRC})
target_link_libraries(worker4_lib PRIVATE Base_Modules spdlog::spdlog)

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE worker4_lib)

#install
install (TARGETS
         consumer DESTINATION ${CMAKE_SOURCE_DIR}/bin)
