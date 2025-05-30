#include <benchmark/benchmark.h>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>

#define GB_IN_BYTES (1024L * 1024 * 1024)
const std::string filePath = "../../Downloads/enwiki.txt"; 
int ProcessFileAndCount(const size_t limitBytes) {
    int count = 0;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return -1;
    }

    std::string line;
    size_t bytesRead = 0;
    while (std::getline(file, line)) {
        bytesRead += line.size() + 1;
        if (bytesRead > limitBytes) break;

        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            count++;
        }
    }
    return count;
}

void ReadFile(const size_t limitBytes) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    std::string line;
    size_t bytesRead = 0;
    while (std::getline(file, line)) {
        bytesRead += line.size() + 1;
        if (bytesRead > limitBytes) break;
    }
}

static void BM_ProcessFileAndCount(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(ProcessFileAndCount(state.range(0)));
    }
}
// BENCHMARK(BM_ProcessFileAndCount)->Arg(3 * GB_IN_BYTES)->Arg(5 * GB_IN_BYTES)->Arg(10 * GB_IN_BYTES)->Arg(20 * GB_IN_BYTES);

static void BM_ReadFile(benchmark::State& state) {
    for (auto _ : state) {
        ReadFile(state.range(0));
    }
}
BENCHMARK(BM_ReadFile)->Arg(3 * GB_IN_BYTES)->Arg(5 * GB_IN_BYTES)->Arg(10 * GB_IN_BYTES)->Arg(20 * GB_IN_BYTES)->Unit(benchmark::kSecond);

BENCHMARK_MAIN();
