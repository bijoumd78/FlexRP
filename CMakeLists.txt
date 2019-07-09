cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(FlexRP LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type set to ${CMAKE_BUILD_TYPE}")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories(./third-party)
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

add_subdirectory(apps)
add_subdirectory(recon_modules)
