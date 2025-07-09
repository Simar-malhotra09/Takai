/*
 * Source for mmap implementation: https://gist.github.com/marcetcheverry/991042
 *
 * */


#include <benchmark/benchmark.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
#include <iostream>
#include <./findWordFreq.h>

namespace mmap {

  FindWordFreq::FindWordFreq(const std::string& path)
    :filePath(path){}

  void FindWordFreq::processFile(const size_t limitByte){
    const char* fPath = filePath.c_str();
    int fd = open(fPath, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Failed to open file: " << strerror(errno) << std::endl;
        return;
    }

    struct stat fileInfo;
    if (fstat(fd, &fileInfo) == -1) {
        std::cerr << "Failed to stat file: " << strerror(errno) << std::endl;
        close(fd);
        return;
    }

    if(fileInfo.st_size==0){
      std::cerr <<"File is empty: " << strerror(errno) << std::endl;
      close(fd);
      return;
    }

    char* data = static_cast<char*>(mmap(nullptr, fileInfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    char* mapStart= data;
    if (data == MAP_FAILED) {
        std::cerr << "mmap failed: " << strerror(errno) << std::endl;
        close(fd);
        return;
    }

    size_t cntr = 0;
    while (cntr < fileInfo.st_size && cntr < limitBytes) {
        int lineLen = 0;
        char* line = data;

        while (*data != '\n' && cntr < fileInfo.st_size && cntr < limitBytes) {
            data++;
            cntr++;
            lineLen++;
        }

    }

    munmap(mapStart, fileInfo.st_size);
    close(fd);
  }


}
