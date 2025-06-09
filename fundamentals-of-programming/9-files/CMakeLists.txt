cmake_minimum_required(VERSION 3.10)
project(RegionSimulator3000 C)

include_directories(${CMAKE_SOURCE_DIR})
enable_testing()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "-g -gdwarf-4")
set(CMAKE_CXX_FLAGS_DEBUG "-g -gdwarf-4")

set(SOURCES
    src/main.c
    src/actions/files.c
    src/actions/misc.c
    src/actions/records.c
    src/actions/regions.c
    src/actions/utils.c
    src/common/fileContext.c
    src/common/sort.c
    src/common/stack.c
    src/io/choices.c
    src/io/number.c
    src/io/string.c
    src/io/utils.c
    src/io/validators.c
    src/menu/default.c
    src/menu/menu.c
)

add_executable(RegionSimulator3000 ${SOURCES})

target_link_libraries(RegionSimulator3000 m)
