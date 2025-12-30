#include <benchmark/benchmark.h>

#include "axia/parser.h"

static void BM_Parser(benchmark::State& state) { // 0.084 ms - laptop
    const std::string script = "myvar = 5 + 3 + ( 5 * 8 * position); test = a / 8 + ( 5 * 8 - 3 + 2);";

    euclide::Parser parser{};
    for (auto _ : state) {
        parser.parse(script);
        benchmark::DoNotOptimize(parser);
    }
}

BENCHMARK(BM_Parser)->Unit(benchmark::kMillisecond);