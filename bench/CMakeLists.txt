add_executable(ButterBench bench_main.cpp bench_grid.cpp)

target_link_libraries(ButterBench PRIVATE core benchmark::benchmark)