
#include "folder_naiveHashMap/findWordFreq.h"  
#include "folder_parallelHashMap/findWordFreq.h"  
#include <chrono>
#include <iostream> 

#define GB_IN_BYTES (1024L * 1024 * 1024)

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./output <file_path>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    // naiveHashMap::FindWordFreq freq_naiveHashMap(filePath);
    parallelHashMap::FindWordFreq freq_parallelHashMap(filePath);
    auto start_processFile= std::chrono::high_resolution_clock::now();

    freq_parallelHashMap.processFile(3*GB_IN_BYTES, 4);

    auto end_processFile = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration_processFile= end_processFile - start_processFile;
    std::cout << "Took " << duration_processFile.count() << " seconds to process file." << std::endl;

    // std::cout << "Total words: " << freq_naiveHashMap.getFileCount() << std::endl;
    // std::cout << "Frequency of 'example': " << freq_naiveHashMap.getWordFreq("example") << std::endl;

    return 0;
}

