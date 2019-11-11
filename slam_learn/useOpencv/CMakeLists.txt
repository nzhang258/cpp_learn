cmake_minimum_required(VERSION 2.8)

project(demo)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(OpenCV 3 REQUIRED)
include_directories(${OpenCV_LIB_DIRS})

add_executable(demo main.cpp)
target_link_libraries(demo ${OpenCV_LIBS})
