#include <benchmark/benchmark.h>
#include "../folder_naiveHashMap/findWordFreq.h"

#define GB_IN_BYTES (1024L * 1024 * 1024)

static void BM_ProcessFile(benchmark::State& state) {
  naiveHashMap::FindWordFreq freq("../Downloads/enwiki.txt");
  size_t bytesToRead = state.range(0); 

  for (auto _ : state) {
    freq.processFile(bytesToRead);
  }
}

BENCHMARK(BM_ProcessFile)->Arg(3 * GB_IN_BYTES)->Unit(benchmark::kSecond);
BENCHMARK(BM_ProcessFile)->Arg(5 * GB_IN_BYTES)->Unit(benchmark::kSecond);
BENCHMARK(BM_ProcessFile)->Arg(10 * GB_IN_BYTES)->Unit(benchmark::kSecond);
BENCHMARK(BM_ProcessFile)->Arg(20 * GB_IN_BYTES)->Unit(benchmark::kSecond);

BENCHMARK_MAIN();

