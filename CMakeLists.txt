cmake_minimum_required(VERSION 2.6)
project(NGAME)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-Wall -pedantic -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -fno-omit-frame-pointer")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# qtcreator 4.3 doesn't parse headers with this
#file(GLOB_RECURSE headers
#    "*.hpp"
#    "*.h"
#    "*.sh")
#add_custom_target(headers SOURCES ${headers})

add_definitions(-DGLM_FORCE_NO_CTOR_INIT -DGLM_ENABLE_EXPERIMENTAL)

include_directories(./include)

if(APPLE)
    # temp
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -std=c++14")
else()
    set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -std=c++17")
endif()

add_subdirectory(src)
add_subdirectory(examples)
