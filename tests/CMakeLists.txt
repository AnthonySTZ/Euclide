add_executable(EuclideTests 
                main.cpp
                geometry/test_attributes.cpp
                geometry/test_mesh.cpp
                
                # nodes/test_node.cpp
                # nodes/test_scene.cpp
                
                # nodes/primitive/test_cube.cpp
                # nodes/primitive/test_grid.cpp
                # nodes/primitive/test_cylinder.cpp
                
                # nodes/geometry/test_transform.cpp
                # nodes/geometry/test_merge.cpp
                
                fields/test_field.cpp
                fields/test_float3.cpp

                )

target_link_libraries(EuclideTests PRIVATE core gtest)

add_test(NAME EuclideTests COMMAND EuclideTests)