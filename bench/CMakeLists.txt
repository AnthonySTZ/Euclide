add_executable(EuclideBench
                bench_main.cpp

                bench_mesh.cpp

                bench_grid.cpp
                bench_cylinder.cpp

                bench_subdivide.cpp
                bench_merge.cpp
                bench_copy_to_points.cpp

                bench_attributes.cpp
                )

target_link_libraries(EuclideBench PRIVATE core benchmark::benchmark)