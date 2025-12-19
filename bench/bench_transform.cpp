#include <benchmark/benchmark.h>

#include "nodes/primitive/quad_sphere.h"
#include "nodes/geometry/transform.h"

static void BM_TranslateQuadSphere11(benchmark::State& state) { // 11.1ms
    euclide::Mesh mesh;
    euclide::QuadSphere::createQuadSphere(mesh, euclide::QuadSphereSettings{.divisions = 11});
    for (auto _ : state) {
        euclide::Transform::translateMesh(mesh, {1.0, -5.0, 2.5});
    }
}

BENCHMARK(BM_TranslateQuadSphere11)->Iterations(20)->Unit(benchmark::kMillisecond);