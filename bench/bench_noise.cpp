#include <benchmark/benchmark.h>

#include "nodes/primitive/grid.h"
#include "nodes/attributes/attribute_noise.h"

static void BM_PerlinNoise(benchmark::State& state) { // 101ms
    euclide::Mesh mesh;
    euclide::Grid::createGrid(mesh, euclide::GridSettings{.divisions = {1000, 1000}});

    euclide::PerlinNoise::PerlinSettings settings{.octaves = 6, .frequency = 1.0};
    for (auto _ : state) {
        euclide::PerlinNoise::applyToMesh(mesh, mesh.pointAttribs, "Cd", 3, settings);
        benchmark::DoNotOptimize(mesh);
    }
}

BENCHMARK(BM_PerlinNoise)->Iterations(10)->Unit(benchmark::kMillisecond);