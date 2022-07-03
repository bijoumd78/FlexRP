cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(FlexRP LANGUAGES CXX C VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wno-sign-compare")
endif()

option(WERROR "Turn warnings into errors" OFF)
if (WERROR)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif ()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
if(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type set to ${CMAKE_BUILD_TYPE}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# User can override the installation directories
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

set(Boost_USE_STATIC_LIBS    OFF)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME OFF)


if(WIN32)
    add_definitions( -D_WIN32_WINNT=0x0601 )
endif()

# We actually only use system and thread explicitly, but they require linking in date_time and chrono
if (WIN32)
    find_package(Boost 1.79.0 COMPONENTS program_options filesystem REQUIRED)
else()
    find_package(Boost 1.79.0 COMPONENTS program_options filesystem REQUIRED)
endif()

find_package(spdlog CONFIG REQUIRED)
find_package(CPPZMQ CONFIG REQUIRED)
find_package(HDF5 REQUIRED)
find_package(FFTW3 CONFIG REQUIRED)
find_package(ISMRMRD REQUIRED)

add_subdirectory(include)
add_subdirectory(apps)
add_subdirectory(recon_modules)

# Testing code
include(CTest)
enable_testing()

if(BUILD_TESTING)
    add_subdirectory(test)
endif(BUILD_TESTING)

# Include extra CMake files for static and dynamic analyzers
include(${CMAKE_SOURCE_DIR}/cmake/clang_tools.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cppcheck.cmake)








