#include <benchmark/benchmark.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
#include <iostream>

#define GB_IN_BYTES (1024L * 1024 * 1024)

inline bool file_exists(const std::string& filePath){
  struct stat buffer;
  return( stat(filePath.c_str(), &buffer)==0);
}
int readFileAndCount(const size_t limitBytes, const std::string& filePath) {
  
  const char* fPath = filePath.c_str();
  int fd = open(fPath, O_RDONLY);
  if (fd == -1) {
      std::cerr << "Failed to open file: " << strerror(errno) << std::endl;
      return -1;
  }

  // http://codewiki.wikidot.com/c:struct-stat
  struct stat fileInfo;
  if (fstat(fd, &fileInfo) == -1) {
      std::cerr << "Failed to stat file: " << strerror(errno) << std::endl;
      close(fd);
      return -1;
  }

  // 
  // reference for mmap function used:https: www.man7.org/linux/man-pages/man2/mmap.2.html
  //
  // void *mmap(void addr[.length], size_t length, int prot, int flags,
  //            int fd, off_t offset);
  //
  // Then we static cast a generic pointer void* returned by mmap, into a pointer char*
  // to get access to raw byte level.
  //
  char* data = static_cast<char*>(mmap(nullptr, fileInfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
  if (data == MAP_FAILED) {
      std::cerr << "mmap failed: " << strerror(errno) << std::endl;
      close(fd);
      return -1;
  }


  size_t cntr = 0;
  size_t wordCount = 0;

  while (cntr < fileInfo.st_size && cntr < limitBytes) {
      int lineLen = 0;
      char* line = data;

      // Capture start of line
      while (*data != '\n' && cntr < fileInfo.st_size && cntr < limitBytes) {
          data++;
          cntr++;
          lineLen++;
      }

      // Count words in this line
      bool inWord = false;
      for (int i = 0; i < lineLen; ++i) {
          char c = line[i];
          if (std::isspace(static_cast<unsigned char>(c))) {
              inWord = false;
          } else if (!inWord) {
              inWord = true;
              ++wordCount;
          }
      }

      benchmark::DoNotOptimize(line);

      // Move past the newline if applicable
      if (cntr < fileInfo.st_size && cntr < limitBytes) {
          data++;
          cntr++;
      }
  }

  // delete mapping
  munmap(data, fileInfo.st_size);  
  close(fd);

  return wordCount;

}


static void BM_ReadWithMMap(benchmark::State& state) {
    const std::string filePath = "/Users/simarmalhotra/Downloads/enwiki.txt";
    if (!file_exists(filePath)) {
        state.SkipWithError(("File does not exist: " + filePath).c_str());
        return;
    }      
    size_t limit = static_cast<size_t>(state.range(0));
    size_t count=0;

    for (auto _ : state) {
        count=readFileAndCount(limit, filePath);
    }
    state.counters["Words"]= count;
}
// BENCHMARK(BM_ReadWithMMap)->Arg(1* GB_IN_BYTES)
                           // ->Unit(benchmark::kSecond) ;

BENCHMARK(BM_ReadWithMMap)->Arg(3 * GB_IN_BYTES)
                           ->Arg(5 * GB_IN_BYTES)
                           ->Arg(10 * GB_IN_BYTES)
                           ->Arg(20 * GB_IN_BYTES)
                           ->Unit(benchmark::kSecond) ;

BENCHMARK_MAIN();
