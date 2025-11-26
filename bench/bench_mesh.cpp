#include <benchmark/benchmark.h>

#include "nodes/primitive/grid.h"

static void BM_HalfedgeConstructionGrid1000x1000(benchmark::State& state) { // 700ms
    butter::Mesh mesh;
    butter::GridSettings settings{
        .divisions = {1000, 1000}
    };
    butter::Grid::createGrid(mesh, settings);

    for (auto _ : state) {
        std::vector<butter::HalfEdge> edges = mesh.computeHalfEdges();
        benchmark::DoNotOptimize(edges);
    }
}

BENCHMARK(BM_HalfedgeConstructionGrid1000x1000)->Iterations(10)->Unit(benchmark::kMillisecond);