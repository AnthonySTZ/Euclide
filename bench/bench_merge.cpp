#include <benchmark/benchmark.h>

#include "nodes/primitive/grid.h"
#include "nodes/geometry/merge.h"

static void BM_MergeGrid1000x1000(benchmark::State& state) { // 87ms
    euclide::Mesh mesh_1;
    euclide::GridSettings settings{.divisions = {1000, 1000}};
    euclide::Grid::createGrid(mesh_1, settings);
    euclide::Mesh mesh_2 = mesh_1;

    for (auto _ : state) {
        euclide::Mesh result = mesh_1;
        euclide::Merge::merge(result, mesh_2);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_MergeGrid1000x1000)->Unit(benchmark::kMillisecond);