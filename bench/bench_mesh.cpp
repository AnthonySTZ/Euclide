#include <benchmark/benchmark.h>

#include "nodes/primitive/grid.h"

static void BM_HalfedgeConstructionGrid10x10(benchmark::State& state) { // 0.002 ms
    euclide::Mesh mesh;
    euclide::GridSettings settings{.divisions = {10, 10}};
    euclide::Grid::createGrid(mesh, settings);

    for (auto _ : state) {
        std::vector<euclide::HalfEdge> edges = mesh.computeHalfEdges();
        benchmark::DoNotOptimize(edges);
    }
}

static void BM_HalfedgeConstructionGrid50x50(benchmark::State& state) { // 0.067 ms
    euclide::Mesh mesh;
    euclide::GridSettings settings{.divisions = {50, 50}};
    euclide::Grid::createGrid(mesh, settings);

    for (auto _ : state) {
        std::vector<euclide::HalfEdge> edges = mesh.computeHalfEdges();
        benchmark::DoNotOptimize(edges);
    }
}

static void BM_HalfedgeConstructionGrid100x100(benchmark::State& state) { // 0.283 ms
    euclide::Mesh mesh;
    euclide::GridSettings settings{.divisions = {100, 100}};
    euclide::Grid::createGrid(mesh, settings);

    for (auto _ : state) {
        std::vector<euclide::HalfEdge> edges = mesh.computeHalfEdges();
        benchmark::DoNotOptimize(edges);
    }
}

static void BM_HalfedgeConstructionGrid200x200(benchmark::State& state) { // 1.81 ms
    euclide::Mesh mesh;
    euclide::GridSettings settings{.divisions = {200, 200}};
    euclide::Grid::createGrid(mesh, settings);

    for (auto _ : state) {
        std::vector<euclide::HalfEdge> edges = mesh.computeHalfEdges();
        benchmark::DoNotOptimize(edges);
    }
}

static void BM_HalfedgeConstructionGrid500x500(benchmark::State& state) { // 13.9 ms
    euclide::Mesh mesh;
    euclide::GridSettings settings{.divisions = {500, 500}};
    euclide::Grid::createGrid(mesh, settings);

    for (auto _ : state) {
        std::vector<euclide::HalfEdge> edges = mesh.computeHalfEdges();
        benchmark::DoNotOptimize(edges);
    }
}

static void BM_HalfedgeConstructionGrid1000x1000(benchmark::State& state) { // 60.2 ms
    euclide::Mesh mesh;
    euclide::GridSettings settings{.divisions = {1000, 1000}};
    euclide::Grid::createGrid(mesh, settings);

    for (auto _ : state) {
        std::vector<euclide::HalfEdge> edges = mesh.computeHalfEdges();
        benchmark::DoNotOptimize(edges);
    }
}

BENCHMARK(BM_HalfedgeConstructionGrid10x10)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_HalfedgeConstructionGrid50x50)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_HalfedgeConstructionGrid100x100)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_HalfedgeConstructionGrid200x200)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_HalfedgeConstructionGrid500x500)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_HalfedgeConstructionGrid1000x1000)->Iterations(20)->Unit(benchmark::kMillisecond);

// std::sort
// BM_HalfedgeConstructionGrid10x10                        0.019 ms        0.019 ms        37333
// BM_HalfedgeConstructionGrid50x50                        0.666 ms        0.663 ms          896
// BM_HalfedgeConstructionGrid100x100                       2.76 ms         2.76 ms          249
// BM_HalfedgeConstructionGrid200x200                       12.3 ms         12.3 ms           56
// BM_HalfedgeConstructionGrid500x500                       83.4 ms         83.3 ms            9
// BM_HalfedgeConstructionGrid1000x1000/iterations:20        353 ms          353 ms           20

// radix
// BM_HalfedgeConstructionGrid10x10                        0.378 ms        0.384 ms         1792
// BM_HalfedgeConstructionGrid50x50                        0.883 ms        0.858 ms          747
// BM_HalfedgeConstructionGrid100x100                       2.29 ms         2.30 ms          299
// BM_HalfedgeConstructionGrid200x200                       8.46 ms         8.51 ms           90
// BM_HalfedgeConstructionGrid500x500                       59.6 ms         59.7 ms           11
// BM_HalfedgeConstructionGrid1000x1000/iterations:20        262 ms          262 ms           20

// std::sort switch to radix at 10k halfedges
// BM_HalfedgeConstructionGrid10x10                        0.019 ms        0.018 ms        37333
// BM_HalfedgeConstructionGrid50x50                        0.646 ms        0.642 ms         1120
// BM_HalfedgeConstructionGrid100x100                       2.27 ms         2.25 ms          299
// BM_HalfedgeConstructionGrid200x200                       8.38 ms         8.33 ms           75
// BM_HalfedgeConstructionGrid500x500                       59.6 ms         59.7 ms           11
// BM_HalfedgeConstructionGrid1000x1000/iterations:20        261 ms          260 ms           20