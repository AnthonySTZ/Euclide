add_executable(ButterTests test_mesh.cpp)

target_link_libraries(ButterTests PRIVATE core gtest gtest_main)

add_test(NAME ButterTests COMMAND ButterTests)