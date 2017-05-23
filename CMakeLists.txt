cmake_minimum_required(VERSION 2.6)
project(NGAME)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -pedantic -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -flto -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g -fno-omit-frame-pointer")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB_RECURSE headers
    "*.hpp"
    "*.h"
    "*.sh")
add_custom_target(headers SOURCES ${headers})

add_definitions(-DGLM_FORCE_NO_CTOR_INIT)

include_directories(./include)

add_subdirectory(src)
add_subdirectory(examples)
