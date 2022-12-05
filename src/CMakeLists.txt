cmake_minimum_required(VERSION 3.17)
project(library)
add_subdirectory(src)
add_subdirectory(Catch2)

set(CMAKE_CXX_STANDARD 14)

add_executable(tests
        src/algorithms/binarySearch/test.cpp
        src/math/modular/tests/test.cpp
)

INCLUDE_DIRECTORIES(Catch2)
INCLUDE_DIRECTORIES(src)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)