cmake_minimum_required(VERSION 2.8)
project(xymalloc)

set(CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)
include_directories(include)

add_executable(test_hugepage test_hugepage.cc)

add_executable(xymalloc main.cc src/alloc.cc src/os.cc)
target_link_libraries(xymalloc PRIVATE Threads::Threads)