#include <benchmark/benchmark.h>

#include "nodes/primitive/cylinder.h"

static void BM_Cylinder100000Div(benchmark::State& state) { // 6 ms
    euclide::CylinderSettings settings{.divisions = 100000};
    for (auto _ : state) {
        euclide::Mesh mesh;
        euclide::Cylinder::createCylinder(mesh, settings);
    }
}

BENCHMARK(BM_Cylinder100000Div)->Iterations(50)->Unit(benchmark::kMillisecond);