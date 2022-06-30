set(SRC
    flexrp_client.h
    flexrp_client.cpp
    )

add_library(FlexRP_client ${SRC})
target_link_libraries(FlexRP_client PUBLIC ${Boost_LIBRARIES} spdlog::spdlog)

add_executable(FlexRP_ismrmrd_client FlexRP_ismrmrd_client.cpp)
target_include_directories(FlexRP_ismrmrd_client PUBLIC ${ZeroMQ_INCLUDE_DIRS} ${ISMRMRD_DIRS})
target_link_libraries(FlexRP_ismrmrd_client PUBLIC ${Boost_LIBRARIES} ${ZeroMQ_LIBRARIES} ISMRMRD::ISMRMRD 
    PRIVATE spdlog::spdlog FlexRP_client)


#install
install (TARGETS
    FlexRP_client
    FlexRP_ismrmrd_client
    ARCHIVE
    DESTINATION ${INSTALL_LIBDIR}
    COMPONENT lib
    RUNTIME
    DESTINATION ${INSTALL_BINDIR}
    COMPONENT bin
    LIBRARY
    DESTINATION ${INSTALL_LIBDIR}
    COMPONENT lib
    PUBLIC_HEADER
    DESTINATION ${INSTALL_INCLUDEDIR}
    COMPONENT dev)
