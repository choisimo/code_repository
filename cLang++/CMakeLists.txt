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
add_executable(cLang__2 final/1107/MyVector.cpp)
add_executable(cLang__3 final/1107/Person.cpp)
add_executable(cLang__4 final/1107/Math.cpp)
add_executable(cLang__5 final/1107/static_member2.cpp)
add_executable(cLang__6 final/1107/static_member_approach.cpp.cpp)
add_executable(cLang__7 report_ch5/exam_1.cpp)
add_executable(cLang__8 report_ch5/exam_11_real.cpp)
add_executable(cLang__9 final/1112/friend.cpp)
add_executable(cLang__10 final/1112/friend2.cpp)
add_executable(cLang__11 final/1112/friend3.cpp)
add_executable(cLang__12 final/1119/ColorPoint.cpp)
add_executable(cLang__13 final/1119/exam_8-2_about_protected.cpp)