add_executable(FleXRP main.cpp)
target_link_libraries(FleXRP PUBLIC pugixml ${Boost_LIBRARIES})

#install
install (TARGETS
         FleXRP
         DESTINATION ${CMAKE_SOURCE_DIR}/bin)
