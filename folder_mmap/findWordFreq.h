#pragma once
#include <string>
#include <unordered_map>

namespace mmap{

  class FindWordFreq {
    private:
      std::string filePath;
      std::unordered_map<std::string, int> freqMap;

    public:
      FindWordFreq(const std::string& path);
      void processFile(size_t limitBytes);
      int getFileCount()const;
      int getWordFreq(const std::string& word) const;

  };

}
