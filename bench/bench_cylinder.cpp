#include <benchmark/benchmark.h>

#include "nodes/primitive/cylinder.h"

static void BM_Cylinder100000Div(benchmark::State& state) { // 69ms
    butter::CylinderSettings settings{
        .divisions = 100000
    };
    for (auto _ : state) {
        butter::Mesh mesh;        
        butter::Cylinder::createCylinder(mesh, settings);
    }
}

BENCHMARK(BM_Cylinder100000Div)->Unit(benchmark::kMillisecond);