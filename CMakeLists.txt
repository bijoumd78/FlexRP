cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(FlexRP LANGUAGES CXX C VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type set to ${CMAKE_BUILD_TYPE}")

include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# User can overriding the installation directories
set(INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR} CACHE PATH "Installation directory for libraries")
set(INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR} CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKEDIR CMake)
else()
    set(DEF_INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME})
endif()
set(INSTALL_CMAKEDIR ${DEF_INSTALL_CMAKEDIR} CACHE PATH "Installation directory for CMake files")

# Print installation directories
foreach(p LIB BIN INCLUDE CMAKE)
    file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${p}DIR} _path )
    message(STATUS "Installing ${p} components to ${_path}")
    unset(_path)
endforeach()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include_directories(./include)
include_directories(/usr/include/hdf5/serial)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
# necessary for Windows and RHEL <=6 systems
set(Boost_NO_BOOST_CMAKE ON)

if(WIN32)
    add_definitions( -DBOOST_ALL_NO_LIB )
    add_definitions( -DBOOST_ALL_DYN_LINK )
    add_definitions( -DBOOST_NO_EXCEPTIONS )
endif()

# We actually only use system and thread explicitly, but they require linking in date_time and chrono
if (WIN32)
    find_package(Boost 1.67.0 COMPONENTS system thread date_time chrono program_options filesystem timer REQUIRED)
else()
    find_package(Boost 1.67.0 COMPONENTS system thread program_options filesystem timer REQUIRED)
endif()

find_package(spdlog REQUIRED)
find_package(ZeroMQ REQUIRED)
find_package(HDF5 1.8 COMPONENTS C CXX HL REQUIRED)
find_package(FFTW3 COMPONENTS single double REQUIRED)
find_package(ISMRMRD REQUIRED)
link_directories(${ISMRMRD_LIBRARY_DIRS})

add_subdirectory(include)
add_subdirectory(apps)
add_subdirectory(recon_modules)
