#include <benchmark/benchmark.h>

#include "nodes/primitive/cube.h"
#include "nodes/geometry/subdivide.h"

static void BM_SubdivideCube8(benchmark::State& state) { // 17.2ms
    butter::SubdivideSettings settings{
        .divisions = 8
    };
    for (auto _ : state) {
        butter::Mesh mesh;
        butter::Cube::createCube(mesh, {});
        butter::Subdivide::subdivide(mesh, settings);
    }
}

BENCHMARK(BM_SubdivideCube8)->Iterations(10)->Unit(benchmark::kMillisecond);