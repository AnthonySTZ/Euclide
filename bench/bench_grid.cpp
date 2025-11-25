#include <benchmark/benchmark.h>

#include "nodes/primitive/grid.h"

#include <numeric>

static void BM_Grid1000x1000(benchmark::State& state) {
    butter::GridSettings settings{
        .divisions = {1000, 1000}
    };
    for (auto _ : state) {
        state.PauseTiming();
        butter::Mesh mesh{};
        state.ResumeTiming();
        
        butter::Grid::createGrid(mesh, settings);
        benchmark::DoNotOptimize(mesh);
    }
}

BENCHMARK(BM_Grid1000x1000);