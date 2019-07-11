add_executable(FlexRP_ismrmrd_client FlexRP_ismrmrd_client.cpp)
target_include_directories(FlexRP_ismrmrd_client PUBLIC ${ZeroMQ_INCLUDE_DIRS} ${ISMRMRD_DIRS})
target_link_libraries(FlexRP_ismrmrd_client PUBLIC ${ZeroMQ_LIBRARIES} ${ISMRMRD_LIBRARIES} ${Boost_LIBRARIES} PRIVATE spdlog::spdlog)


#install
install (TARGETS
	FlexRP_ismrmrd_client
         DESTINATION ${CMAKE_SOURCE_DIR}/bin)
