cmake_minimum_required(VERSION 3.10) 
project(FindingRoots C)                

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCES
    src/main.c
    src/algorithm.c
    src/io/choices.c
    src/io/utils.c
    src/io/double.c
    src/io/int.c
    src/io/validators.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fblocks")

add_executable(main ${SOURCES})
target_link_libraries(main m BlocksRuntime)
