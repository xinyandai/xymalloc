cmake_minimum_required(VERSION 3.0)
project(xymalloc)

set(CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)
include_directories(include)

set (LINK_LIBS ${LINK_LIBS} Threads::Threads numa )

file(GLOB xy_malloc_src
        src/*.cc)
add_library(xy_malloc_lib ${xy_malloc_src})

SET(tests
        huge_page
        numa_page_migrate
        realloc_test
        stack_alloc_test)

FOREACH(test ${tests})
add_executable(${test} test/${test}.cc)
target_link_libraries(${test} PRIVATE ${LINK_LIBS} xy_malloc_lib)
ENDFOREACH(EXAMPLE)

