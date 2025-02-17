CMAKE_MINIMUM_REQUIRED(VERSION 3.12.1)
set(CMAKE_CXX_STANDARD 14)

PROJECT(openfsm)


include_directories(src)
include_directories(test)

add_executable(test
        openfsm.h
        openfsm.cpp
        main.cpp
)
