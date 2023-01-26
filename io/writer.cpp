#include "writer.h"

//TODO: Remove this
#include <iostream>

Writer::Writer(const char* file, char type){
    this->file = file;
    memset(buffer, 0, sizeof buffer);
    this->out.open(file, std::ios::binary);
    this->type = type;
    // reserve space for file length
    if (type == 2){
        for (int i = 0; i < sizeof(long long); i++){
            this->pushChar(0);
        }
        this->total = 0;
    }
}

Writer::~Writer(){};

void Writer::push(long long x, char len){
    x &= (1LL << len) - 1;
    if (len + this->resBits >= 8){
        char sh = (8 - this->resBits);
        this->pushChar((this->residual << sh) + ((unsigned long long)x >> (len - sh)));
        x &= (1LL << (len - sh)) - 1;
        len -= sh;
        this->residual = 0;
        this->resBits = 0;
    }
    while (len >= 8){
        this->pushChar((unsigned long long)x >> (len - 8));
        x &= (1LL << (len - 8)) - 1;
        len -= 8;
    }
    this->residual = (this->residual << len) + x;
    this->resBits += len;
}

void Writer::pushChar(char x){
    if (this->pos == this->BUFFER_SIZE){
        this->out.write(this->buffer, this->BUFFER_SIZE);
        this->pos = 0;
    }
    this->buffer[this->pos++] = x;
    this->total += 8;
}

void Writer::close(){
    if (this->resBits != 0){
        this->pushChar(this->residual << (8 - this->resBits));
        this->total -= 8 - this->resBits;
    }
    this->out.write(this->buffer, this->pos);

    this->pos = 0;
    if(this->type == 2){
        this->prepend(this->total);
    }
    this->out.close();
    this->~Writer();
}

void Writer::prepend(long long x){   ///TODO: UNSAFE
    this->out.close();
    this->out.open(this->file, std::fstream::in | std::fstream::out | std::fstream::binary);
    for (int i = 7; i >= 0; i--){
        char c = (unsigned long long)x >> (i * 8);
        this->out.write(&c, 1);
        x &= (1LL << (i * 8)) - 1;
    }
}