include_directories(../core)

add_executable(consumer consumer.cpp)
#target_include_directories(consumer PUBLIC ${ZeroMQ_INCLUDE_DIRS} ${ISMRMRD_DIRS})
#target_link_libraries(consumer PUBLIC ${ZeroMQ_LIBRARIES} ${ISMRMRD_LIBRARIES} PRIVATE spdlog::spdlog)
target_link_libraries(consumer PRIVATE Base_Modules spdlog::spdlog)

#install
install (TARGETS
         consumer DESTINATION ${CMAKE_SOURCE_DIR}/bin)
