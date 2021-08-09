cmake_minimum_required(VERSION 3.0)
project(xymalloc)

set(CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)
include_directories(include)

set (LINK_LIBS ${LINK_LIBS} Threads::Threads numa )

add_executable(huge_page test/huge_page.cc src/os.cc)

add_executable(numa_page_migrate test/numa_page_migrate.cc)
target_link_libraries(numa_page_migrate PRIVATE ${LINK_LIBS})



add_executable(realloc_test test/realloc_test.cc)
target_link_libraries(realloc_test PRIVATE ${LINK_LIBS})

add_executable(xymalloc app/main.cc src/alloc.cc src/os.cc)

target_link_libraries(xymalloc PRIVATE ${LINK_LIBS})
