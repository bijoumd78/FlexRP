set(SRC
	Base_Modules.h
	Base_Modules.cpp
    )

add_library(Base_Modules ${SRC})
target_include_directories(Base_Modules PUBLIC ${ZeroMQ_INCLUDE_DIRS} ${ISMRMRD_DIRS})
target_link_libraries(Base_Modules PUBLIC ${ZeroMQ_LIBRARIES} ${ISMRMRD_LIBRARIES} PRIVATE spdlog::spdlog)
