#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#define _TINY_UTF8_IMPL_
#include "tiny_utf8.h"

std::vector<tiny_utf8_rune> readFile(const std::string& fileName) {
    std::ifstream inFile(fileName);
    if (!inFile) {
        std::cerr << "error: failed to open in file\n";
        exit(1);
    }

    std::vector<tiny_utf8_rune> runes;

    std::string line;
    tiny_utf8_rune rune;
    while (std::getline(inFile, line)) {
        line.push_back('\n');

        size_t i = 0;
        while (i < line.size()) {
            i += tiny_utf8_decode(&rune, line.c_str() + i, line.size() - i);
            runes.push_back(rune);
        }
    }

    inFile.close();

    return runes;
}

void writeFile(const std::string& fileName, std::vector<tiny_utf8_rune>& runes) {
    std::ofstream outFile(fileName);
    if (!outFile) {
        std::cerr << "error: failed to open out file\n";
        exit(1);
    }
    
    char buff[4];

    for (auto rune : runes) {
        size_t n = tiny_utf8_encode(rune, buff, 4);
        outFile.write(buff, n);
    }

    outFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "error: usage: tiny_utf8 <path>\n";
        exit(1);
    }

    std::string fileName = argv[1];
    auto runes = readFile(fileName);

    for (auto rune : runes) {
        printf("%x ", rune);
    }
    std::cout << '\n';

    writeFile(fileName + ".out", runes);
    return 0;
}