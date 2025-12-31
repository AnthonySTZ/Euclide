#include <benchmark/benchmark.h>

#include "nodes/primitive/grid.h"
#include "nodes/attributes/attribute_noise.h"
#include "nodes/attributes/attribute_axia.h"

static void BM_AXIAGrid1000x1000(benchmark::State& state) { // 300 ms - laptop
    euclide::GridSettings gridSettings{.divisions = {1000, 1000}};
    euclide::Mesh mesh;
    euclide::Grid::createGrid(mesh, gridSettings);
    euclide::PerlinNoise::PerlinSettings perlinSettings{.octaves = 6, .frequency = 1.0};
    euclide::PerlinNoise perlin{};
    perlin.applyToMesh(mesh, mesh.pointAttribs, "height", 1, perlinSettings);

    const std::string script = "float amp = 0.5;"
                               "float noise = f@height * amp;"
                               "v@P.y = noise;";

    for (auto _ : state) {
        euclide::AttributeAXIA::computeAXIA(mesh.pointAttribs, script);
        benchmark::DoNotOptimize(mesh);
    }
}

BENCHMARK(BM_AXIAGrid1000x1000)->Iterations(50)->Unit(benchmark::kMillisecond);