cmake_minimum_required(VERSION 3.27)
project(cLang__)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(middle/0905)

add_executable(cLang__
        middle/0905/hello.cpp
        middle/0905/name.cpp
        middle/0905/nameCard.cpp)
add_executable(cLang__1 final/1105/Person.cpp)
