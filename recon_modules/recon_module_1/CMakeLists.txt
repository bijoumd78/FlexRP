include_directories(../core)

add_executable(worker1 worker1.cpp)
#target_include_directories(worker1 PUBLIC ${ZeroMQ_INCLUDE_DIRS} ${ISMRMRD_DIRS})
#target_link_libraries(worker1 PUBLIC ${ZeroMQ_LIBRARIES} ${ISMRMRD_LIBRARIES} PRIVATE spdlog::spdlog)
target_link_libraries(worker1 PRIVATE Base_Modules spdlog::spdlog)

#install
install (TARGETS
         worker1 DESTINATION ${CMAKE_SOURCE_DIR}/bin)
