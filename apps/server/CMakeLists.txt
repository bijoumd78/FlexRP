set( SOURCES
             Flexrp_xml.h
             Flexrp_xml.cpp
    )

add_library(Flexrp_xml ${SOURCES})
target_link_libraries(Flexrp_xml PUBLIC pugixml ${Boost_LIBRARIES})

add_executable(FleXRP main.cpp)
target_link_libraries(FleXRP PUBLIC Flexrp_xml)

#install
install (TARGETS
         FleXRP
         DESTINATION ${CMAKE_SOURCE_DIR}/bin)
