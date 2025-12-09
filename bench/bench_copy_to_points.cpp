#include <benchmark/benchmark.h>

#include "nodes/primitive/grid.h"
#include "nodes/primitive/quad_sphere.h"
#include "nodes/geometry/copy_to_points.h"

static void BM_CopyToPoints(benchmark::State& state) { // 4679 ms
    euclide::Mesh mesh_1;
    euclide::GridSettings settings{.divisions = {20, 20}};
    euclide::Grid::createGrid(mesh_1, settings);
    euclide::Mesh mesh_2;
    euclide::QuadSphereSettings quadSettings{.divisions = 8};
    euclide::QuadSphere::createQuadSphere(mesh_2, quadSettings);

    for (auto _ : state) {
        euclide::Mesh result = mesh_2;
        euclide::CopyToPoints::copyToPoints(result, mesh_1);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_CopyToPoints)->Iterations(5)->Unit(benchmark::kMillisecond);