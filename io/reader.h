#pragma once
#include <cstdlib>
#include <fstream>
#include <utility>

class Reader {
    std::ifstream in;
    const int BUFFER_SIZE = 4096;  //4kB
    char *buffer = (char*)malloc(BUFFER_SIZE * sizeof(char));
    int pos = BUFFER_SIZE;  // cursor position
    int limit = BUFFER_SIZE;// last read character in buffer
    long long residual = 0;
    char resBits = 0;
    public:
        Reader(const char *s);
        long long get(char len);
        char getChar();
        void close();
        std::pair<int, int> flush();
};