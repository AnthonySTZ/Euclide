#include <benchmark/benchmark.h>

#include "nodes/primitive/grid.h"

static void BM_Grid1000x1000(benchmark::State& state) { // 69ms
    euclide::GridSettings settings{.divisions = {1000, 1000}};
    for (auto _ : state) {
        euclide::Mesh mesh;
        euclide::Grid::createGrid(mesh, settings);
    }
}

BENCHMARK(BM_Grid1000x1000)->Iterations(50)->Unit(benchmark::kMillisecond);