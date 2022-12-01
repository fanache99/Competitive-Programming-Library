cmake_minimum_required(VERSION 3.17)
project(library)
add_subdirectory(algorithms)
add_subdirectory(Catch2)

set(CMAKE_CXX_STANDARD 14)

add_executable(tests
        algorithms/binarySearch/test.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)