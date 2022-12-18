cmake_minimum_required(VERSION 3.17)
project(library)

set(CMAKE_CXX_STANDARD 17)


add_executable(tests
        src/algorithms/test.cpp
        src/math/tests/test_modular.cpp
)
add_executable(main main.cpp)

INCLUDE_DIRECTORIES(Catch2)
INCLUDE_DIRECTORIES(src)
INCLUDE_DIRECTORIES(/usr/local/include)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
add_subdirectory(src)
add_subdirectory(Catch2)
add_subdirectory(problems)