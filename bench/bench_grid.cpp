#include <benchmark/benchmark.h>

#include "nodes/primitive/grid.h"

static void BM_Grid1000x1000(benchmark::State& state) { // 5.30 ms
    euclide::GridSettings settings{.divisions = {1000, 1000}};
    for (auto _ : state) {
        euclide::Mesh mesh;
        euclide::Grid::createGrid(mesh, settings);
    }
}

static void BM_Grid10000x10000(benchmark::State& state) { // 569 ms
    euclide::GridSettings settings{.divisions = {10000, 10000}};
    for (auto _ : state) {
        euclide::Mesh mesh;
        euclide::Grid::createGrid(mesh, settings);
    }
}

BENCHMARK(BM_Grid1000x1000)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_Grid10000x10000)->Iterations(10)->Unit(benchmark::kMillisecond);