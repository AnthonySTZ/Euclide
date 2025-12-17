#include <benchmark/benchmark.h>

#include "nodes/primitive/quad_sphere.h"
#include "nodes/attributes/attribute_create.h"

static void BM_CopyAttribute(benchmark::State& state) {
    euclide::TypedAttribute<float, 3> P("P");
    P.resize(9'999'999);

    for (auto _ : state) {
        euclide::TypedAttribute<float, 3> Cd = P;
        benchmark::DoNotOptimize(Cd);
    }
}

static void BM_CreateAttribute(benchmark::State& state) {
    euclide::Mesh mesh;
    euclide::QuadSphereSettings settings{.divisions = 10};
    euclide::QuadSphere::createQuadSphere(mesh, settings);

    for (auto _ : state) {
        euclide::AttributeSet attrbs = mesh.pointAttribs;
        euclide::AttributeCreate::createAttribute(attrbs, "Cd", 3, {3.0, 2.0, 1.0, 0.0});
        benchmark::DoNotOptimize(attrbs);
    }
}

BENCHMARK(BM_CopyAttribute)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_CreateAttribute)->Unit(benchmark::kMillisecond);