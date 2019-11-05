cmake_minimum_required(VERSION 2.8)

project(HelloWorld)

set(CMAKE_BUILD_TYPE "Debug")



add_library(libhello libHello.cpp)
add_library(libhello_shared SHARED libHello.cpp)

add_executable(useHello useHello.cpp)
target_link_libraries(useHello libhello)
#target_link_libraries(useHello libhello_shared)
