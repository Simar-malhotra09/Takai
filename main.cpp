#include <iostream>
#include "findWordFreq.h"  

int main() {
    FindWordFreq freq("../Downloads/smpl_enwiki.txt");
    freq.processFile();

    std::cout << "Total words: " << freq.getFileCount() << std::endl;
    std::cout << "Frequency of 'example': " << freq.getWordFreq("example") << std::endl;

    return 0;
}
