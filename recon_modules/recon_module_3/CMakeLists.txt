include_directories(../core)

add_executable(worker3 worker3.cpp)
#target_include_directories(worker3 PUBLIC ${ZeroMQ_INCLUDE_DIRS} ${ISMRMRD_DIRS})
#target_link_libraries(worker3 PUBLIC ${ZeroMQ_LIBRARIES} ${ISMRMRD_LIBRARIES} PRIVATE spdlog::spdlog)
target_link_libraries(worker3 PRIVATE Base_Modules spdlog::spdlog)

#install
install (TARGETS
         worker3 DESTINATION ${CMAKE_SOURCE_DIR}/bin)
