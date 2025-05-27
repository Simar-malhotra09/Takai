
#include <benchmark/benchmark.h>
#include "../folder_naiveHashMap/findWordFreq.h"

static void BM_ProcessFile(benchmark::State& state) {
  naiveHashMap::FindWordFreq freq("../Downloads/smpl_enwiki.txt");
  for (auto _ : state) {
    freq.processFile();
  }
}

BENCHMARK(BM_ProcessFile);
BENCHMARK_MAIN();
