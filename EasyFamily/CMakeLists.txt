cmake_minimum_required(VERSION 3.10)
project(EasyFamily)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR})

# Find all source files
file(GLOB SOURCES "*.cpp")

# Create library
add_library(EasyFamily ${SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(EasyFamily PUBLIC Threads::Threads)

enable_testing()
add_subdirectory(test)
