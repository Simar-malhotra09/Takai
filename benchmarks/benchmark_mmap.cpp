#include <benchmark/benchmark.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
#include <iostream>

#define GB_IN_BYTES (1024L * 1024 * 1024)

void readFile(const size_t limitBytes, const std::string& filePath) {
    const char* fPath = filePath.c_str();
    int fd = open(fPath, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Failed to open file: " << strerror(errno) << std::endl;
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        std::cerr << "Failed to stat file: " << strerror(errno) << std::endl;
        close(fd);
        return;
    }

    char* data = static_cast<char*>(mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (data == MAP_FAILED) {
        std::cerr << "mmap failed: " << strerror(errno) << std::endl;
        close(fd);
        return;
    }

    size_t cntr = 0;
    while (cntr < sb.st_size && cntr < limitBytes) {
        int lineLen = 0;
        char* line = data;

        while (*data != '\n' && cntr < sb.st_size && cntr < limitBytes) {
            data++;
            cntr++;
            lineLen++;
        }

        benchmark::DoNotOptimize(line);
        if (cntr < sb.st_size && cntr < limitBytes) {
            data++;
            cntr++;
        }
    }

    munmap(data, sb.st_size);
    close(fd);
}

static void BM_ReadWithMMap(benchmark::State& state) {
    const std::string filePath = "../../Downloads/enwiki.txt";
    size_t limit = static_cast<size_t>(state.range(0));
    for (auto _ : state) {
        readFile(limit, filePath);
    }
}

BENCHMARK(BM_ReadWithMMap)->Arg(3 * GB_IN_BYTES)
                           ->Arg(5 * GB_IN_BYTES)
                           ->Arg(10 * GB_IN_BYTES)
                           ->Arg(20 * GB_IN_BYTES)
                           ->Unit(benchmark::kSecond) ;

BENCHMARK_MAIN();
