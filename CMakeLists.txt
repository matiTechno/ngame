cmake_minimum_required(VERSION 2.6)
project(NGAME)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -pedantic -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -03 -g")

file(GLOB_RECURSE headers
    "*.hpp"
    "*.h"
    "*.sh")
add_custom_target(headers SOURCES ${headers})

add_definitions(-DGLM_FORCE_NO_CTOR_INIT)

include_directories(./include)

add_subdirectory(src)
add_subdirectory(examples)
