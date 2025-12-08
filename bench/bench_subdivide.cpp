#include <benchmark/benchmark.h>

#include "nodes/primitive/cube.h"
#include "nodes/geometry/subdivide.h"

static void BM_SubdivideCube8(benchmark::State& state) { // 38ms
    euclide::SubdivideSettings settings{.divisions = 8};
    for (auto _ : state) {
        euclide::Mesh mesh;
        euclide::Cube::createCube(mesh, {});
        euclide::Subdivide::subdivide(mesh, settings);
    }
}

static void BM_SubdivideCube9(benchmark::State& state) { // 160ms
    euclide::SubdivideSettings settings{.divisions = 9};
    for (auto _ : state) {
        euclide::Mesh mesh;
        euclide::Cube::createCube(mesh, {});
        euclide::Subdivide::subdivide(mesh, settings);
    }
}

static void BM_SubdivideCube10(benchmark::State& state) { // 656 ms
    euclide::SubdivideSettings settings{.divisions = 10};
    for (auto _ : state) {
        euclide::Mesh mesh;
        euclide::Cube::createCube(mesh, {});
        euclide::Subdivide::subdivide(mesh, settings);
    }
}

BENCHMARK(BM_SubdivideCube8)->Iterations(10)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_SubdivideCube9)->Iterations(10)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_SubdivideCube10)->Iterations(10)->Unit(benchmark::kMillisecond);