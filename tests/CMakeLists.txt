add_executable(ButterTests)

target_link_libraries(ButterTests PRIVATE Butter gtest gtest_main)

add_test(NAME ButterTests COMMAND ButterTests)