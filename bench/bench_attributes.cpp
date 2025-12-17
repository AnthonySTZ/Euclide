#include <benchmark/benchmark.h>

#include "nodes/primitive/quad_sphere.h"
#include "nodes/attributes/attribute_create.h"
#include "nodes/attributes/attribute_randomize.h"

static void BM_CopyAttribute(benchmark::State& state) { // 11.1ms
    euclide::TypedAttribute<float, 3> P("P");
    P.resize(9'999'999);

    for (auto _ : state) {
        euclide::TypedAttribute<float, 3> Cd = P;
        benchmark::DoNotOptimize(Cd);
    }
}

static void BM_CreateAttribute(benchmark::State& state) { // 24.7ms
    euclide::Mesh mesh;
    euclide::QuadSphereSettings settings{.divisions = 10};
    euclide::QuadSphere::createQuadSphere(mesh, settings);

    for (auto _ : state) {
        euclide::AttributeSet attrbs = mesh.pointAttribs;
        euclide::AttributeCreate::createAttribute(attrbs, "Cd", 3, {3.0, 2.0, 1.0, 0.0});
        benchmark::DoNotOptimize(attrbs);
    }
}

static void BM_RandomizeAttribute(benchmark::State& state) { // 37ms
    euclide::Mesh mesh;
    euclide::QuadSphereSettings settings{.divisions = 10};
    euclide::QuadSphere::createQuadSphere(mesh, settings);

    for (auto _ : state) {
        euclide::AttributeSet attrbs = mesh.pointAttribs;
        euclide::AttributeRandomize::randomizeAttribute(attrbs, "Cd", 3, {0.0, 0.0, 0.0, 0.0}, {1.0, 1.0, 1.0, 1.0},
                                                        0.0f);
        benchmark::DoNotOptimize(attrbs);
    }
}

BENCHMARK(BM_CopyAttribute)->Iterations(10)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_CreateAttribute)->Iterations(10)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_RandomizeAttribute)->Iterations(10)->Unit(benchmark::kMillisecond);