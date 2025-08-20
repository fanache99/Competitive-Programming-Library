cmake_minimum_required(VERSION 3.17)
project(library)

set(CMAKE_CXX_STANDARD 20)
ADD_DEFINITIONS(-DLOCAL_PROJECT=1)


add_executable(tests
        src/algorithms/test.cpp
        src/math/tests/test_modular.cpp
)
add_executable(main main.cpp)

INCLUDE_DIRECTORIES(Catch2)
INCLUDE_DIRECTORIES(src)
INCLUDE_DIRECTORIES(/usr/local/include)
INCLUDE_DIRECTORIES(/Library/Developer/CommandLineTools/SDKs/MacOSX14.sdk/usr/include)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
add_subdirectory(src)
add_subdirectory(Catch2)
add_subdirectory(problems)