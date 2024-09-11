cmake_minimum_required(VERSION 3.27)
project(cLang__)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(0905)

add_executable(cLang__
        test.cpp
        test.h
        0905/hello.cpp
        0905/name.cpp
        0905/nameCard.cpp)
