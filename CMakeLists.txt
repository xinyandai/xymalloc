cmake_minimum_required(VERSION 2.8)
project(xymalloc)

set(CMAKE_CXX_STANDARD 14)

include_directories(xymalloc include)
add_executable(xymalloc main.cc src/alloc.c)
find_package(Threads REQUIRED)
target_link_libraries(xymalloc PRIVATE Threads::Threads)