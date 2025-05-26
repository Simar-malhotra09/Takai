#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "findWordFreq.h"

namespace naiveHashMap{

  FindWordFreq::FindWordFreq(const std::string& path)
    :filePath(path){}

  void FindWordFreq::processFile(){
    std::ifstream file(filePath);
    if(file.is_open()){
      std::string line;
      while(std::getline(file, line)){
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        std::istringstream iss(line);
        std::string word;
        while(iss >> word){
          freqMap[word]++;

        }

        
      }
    }
    else{
      std::cerr<<" Failed to open file:" <<filePath <<std::endl;

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
