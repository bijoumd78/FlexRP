include_directories(../core)

add_executable(worker2 worker2.cpp)
#target_include_directories(worker2 PUBLIC ${ZeroMQ_INCLUDE_DIRS} ${ISMRMRD_DIRS})
#target_link_libraries(worker2 PUBLIC ${ZeroMQ_LIBRARIES} ${ISMRMRD_LIBRARIES} PRIVATE spdlog::spdlog)
target_link_libraries(worker2 PRIVATE Base_Modules spdlog::spdlog)

#install
install (TARGETS
         worker2 DESTINATION ${CMAKE_SOURCE_DIR}/bin)
