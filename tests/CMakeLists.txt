add_executable(ButterTests 
                geometry/test_points.cpp
                geometry/test_mesh.cpp
                
                nodes/test_node.cpp
                nodes/test_scene.cpp
                
                nodes/primitive/test_cube.cpp
                nodes/primitive/test_grid.cpp
                
                nodes/geometry/test_transform.cpp
                
                fields/test_field.cpp
                fields/test_float3.cpp

                )

target_link_libraries(ButterTests PRIVATE core gtest gtest_main)

add_test(NAME ButterTests COMMAND ButterTests)