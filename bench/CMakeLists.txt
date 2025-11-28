add_executable(ButterBench
                bench_main.cpp

                bench_mesh.cpp

                bench_grid.cpp
                bench_cylinder.cpp

                bench_subdivide.cpp
                )

target_link_libraries(ButterBench PRIVATE core benchmark::benchmark)