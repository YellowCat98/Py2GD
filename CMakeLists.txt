cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(PythonThingIDK VERSION 1.0.0)

# Set up the mod binary
file(GLOB SOURCES
    src/*.cpp
    src/*.hpp
    src/hooks/*.cpp
    )

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

set(FETCHCONTENT_TIMEOUT 100000000)

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
CPMAddPackage(
    NAME pybind11
    GITHUB_REPOSITORY pybind/pybind11
    VERSION 2.9.0 # specify the version you need
)

target_link_libraries(${PROJECT_NAME} pybind11::embed pybind11::module)

setup_geode_mod(${PROJECT_NAME})