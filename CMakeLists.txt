cmake_minimum_required(VERSION 2.6)
project(NGAME)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -pedantic -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

file(GLOB_RECURSE headers
    "*.hpp"
    "*.h")
add_custom_target(headers SOURCES ${headers})

include_directories(./include)

add_subdirectory(src)
