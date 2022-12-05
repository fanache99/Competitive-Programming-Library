add_executable(test_modular tests/test.cpp)
target_link_libraries(test_modular PRIVATE Catch2::Catch2WithMain)