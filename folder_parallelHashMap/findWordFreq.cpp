#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>
#include "./findWordFreq.h"

namespace parallelHashMap{

  FindWordFreq::FindWordFreq(const std::string& path)
    :filePath(path){}

  void FindWordFreq::processFile(size_t limitBytes, size_t numThreads ) {
      std::ifstream file(this->filePath, std::ios::ate | std::ios::binary);
      if (!file.is_open()) {
          std::cerr << "Failed to open file: " << this->filePath << std::endl;
          return;
      }

      size_t fileSize = file.tellg();
      if (limitBytes > 0 && limitBytes < fileSize)
          fileSize = limitBytes;
      file.close();

      size_t blockSize = fileSize / numThreads;

      std::vector<std::unordered_map<std::string, int>> localMaps(numThreads);
      std::vector<std::thread> threads;

      auto worker = [&](size_t threadIndex) {
          size_t start = threadIndex * blockSize;
          size_t end = (threadIndex == numThreads - 1) ? fileSize : (start + blockSize);

          std::ifstream threadFile(this->filePath);
          if (!threadFile.is_open()) {
              std::cerr << "Thread " << threadIndex << " failed to open file." << std::endl;
              return;
          }

          threadFile.seekg(start);

          if (start != 0) {
              std::string dummy;
              std::getline(threadFile, dummy);
          }

          std::string line;
          size_t bytesRead = start;

          while (bytesRead < end && std::getline(threadFile, line)) {
              bytesRead += line.size() + 1; // +1 for newline char

              if (bytesRead > end)
                  break;

              std::transform(line.begin(), line.end(), line.begin(), ::tolower);
              std::istringstream iss(line);
              std::string word;
              while (iss >> word) {
                  localMaps[threadIndex][word]++;
              }
          }
      };

        for (size_t i = 0; i < numThreads; i++) {
            threads.emplace_back(worker, i);
        }

        for (auto& t : threads) {
            t.join();
        }

        for (auto& localMap : localMaps) {
            for (auto& kv : localMap) {
                this->freqMap[kv.first] += kv.second;
            }
        }
    }
  int FindWordFreq::getFileCount() const {
    int totalCount= 0;
    for(const auto& it: freqMap){
      totalCount+= it.second;
    }

    return totalCount;
  }

  int FindWordFreq::getWordFreq(const std::string& word)const{
    auto it = freqMap.find(word);
    if(it== freqMap.end()){
      return 0;
    }
    return it->second;
  }

}
