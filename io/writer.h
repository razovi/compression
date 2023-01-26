#pragma once
#include <fstream>
#include <cstring>

class Writer {
    const char *file;
    char type;  // 1 -> compressed;  2 -> uncompressed
    std::ofstream out;
    int BUFFER_SIZE = 4096;
    char *buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
    int pos = 0;
    long long residual = 0;
    char resBits = 0;
    int total = 0;
    void prepend(long long x);
    public:
        Writer(const char* file, char type);
        ~Writer();
        void push(long long x, char len);
        void pushChar(char x);
        void close();
};