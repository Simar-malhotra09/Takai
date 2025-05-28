#include <benchmark/benchmark.h>
#include "../folder_parallelHashMap/findWordFreq.h"

#define GB_IN_BYTES (1024L * 1024 * 1024)

static void BM_ProcessFile(benchmark::State& state) {
  size_t bytesToRead = state.range(0);
  size_t numThreads = state.range(1);

  for (auto _ : state) {
    parallelHashMap::FindWordFreq freq("../Downloads/enwiki.txt");
    freq.processFile(bytesToRead, numThreads);
  }
}

BENCHMARK(BM_ProcessFile)->Args({3 * GB_IN_BYTES, 4})->Unit(benchmark::kSecond);
BENCHMARK(BM_ProcessFile)->Args({5 * GB_IN_BYTES, 4})->Unit(benchmark::kSecond);
BENCHMARK(BM_ProcessFile)->Args({10 * GB_IN_BYTES, 4})->Unit(benchmark::kSecond);
BENCHMARK(BM_ProcessFile)->Args({20 * GB_IN_BYTES, 4})->Unit(benchmark::kSecond);

BENCHMARK_MAIN();
