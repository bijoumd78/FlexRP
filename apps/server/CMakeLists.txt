set( SOURCES
    Flexrp_xml.h
    Flexrp_xml.cpp
    )

add_library(Flexrp_xml ${SOURCES})
target_link_libraries(Flexrp_xml PUBLIC pugixml ${Boost_LIBRARIES} PRIVATE spdlog::spdlog FlexSharedMemory)

add_executable(FleXRP main.cpp)
target_link_libraries(FleXRP PUBLIC Flexrp_xml)

#install
install (TARGETS
    Flexrp_xml
    FleXRP
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
