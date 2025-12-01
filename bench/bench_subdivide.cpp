#include <benchmark/benchmark.h>

#include "nodes/primitive/cube.h"
#include "nodes/geometry/subdivide.h"

static void BM_SubdivideCube8(benchmark::State& state) { // 17.2ms
    euclide::SubdivideSettings settings{.divisions = 8};
    for (auto _ : state) {
        euclide::Mesh mesh;
        euclide::Cube::createCube(mesh, {});
        euclide::Subdivide::subdivide(mesh, settings);
    }
}

BENCHMARK(BM_SubdivideCube8)->Iterations(10)->Unit(benchmark::kMillisecond);