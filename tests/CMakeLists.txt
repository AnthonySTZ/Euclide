add_executable(ButterTests test_points.cpp nodes/test_node.cpp nodes/test_scene.cpp)

target_link_libraries(ButterTests PRIVATE core gtest gtest_main)

add_test(NAME ButterTests COMMAND ButterTests)