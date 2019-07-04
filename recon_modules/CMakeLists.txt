add_executable(worker1 worker1.cpp)
target_include_directories(worker1 PUBLIC ${ZeroMQ_INCLUDE_DIRS} ${ISMRMRD_DIRS})
target_link_libraries(worker1 PUBLIC ${ZeroMQ_LIBRARIES} ${ISMRMRD_LIBRARIES})

add_executable(worker2 worker2.cpp)
target_include_directories(worker2 PUBLIC ${ZeroMQ_INCLUDE_DIRS} ${ISMRMRD_DIRS})
target_link_libraries(worker2 PUBLIC ${ZeroMQ_LIBRARIES} ${ISMRMRD_LIBRARIES})

add_executable(worker3 worker3.cpp)
target_include_directories(worker3 PUBLIC ${ZeroMQ_INCLUDE_DIRS} ${ISMRMRD_DIRS})
target_link_libraries(worker3 PUBLIC ${ZeroMQ_LIBRARIES} ${ISMRMRD_LIBRARIES})

add_executable(consumer consumer.cpp)
target_include_directories(consumer PUBLIC ${ZeroMQ_INCLUDE_DIRS} ${ISMRMRD_DIRS})
target_link_libraries(consumer PUBLIC ${ZeroMQ_LIBRARIES} ${ISMRMRD_LIBRARIES})

#install
install (TARGETS
         worker1
         worker2
         worker3
         consumer DESTINATION ${CMAKE_SOURCE_DIR}/bin)
