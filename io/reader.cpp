#include "reader.h"
#include <stdexcept>

Reader::Reader(const char* file){
    this->in.open(file, std::ios::binary);
}

long long Reader::get(char len){
    if (len < 1 || len > 64)
        throw std::out_of_range("Can't read less than 1 bit or more than 32 bits of data at once");
    if (this->resBits >= len){
        char sh = this->resBits - len;
        long long r = (unsigned long long)this->residual >> sh;
        this->residual &= (1LL << sh) - 1;
        this->resBits = sh;
        return r;
    }

    unsigned char next;
    while (len > this->resBits + 8){
        try{
            next = this->getChar();
        }
        catch (...){
            throw;
        }
        this->residual = (this->residual << 8) + next;
        this->resBits += 8;
    }
    try{
        next = this->getChar();
    }
    catch (...){
        throw;
    }
    char rbit = len - this->resBits;
    char charbit = 8 - rbit;
    long long r = (this->residual << rbit) + ((unsigned char)next >> charbit);
    this->residual = next & ((1LL << charbit) - 1);
    this->resBits = charbit;
    return r;
}

char Reader::getChar(){
    while (this->pos == this->limit){
        if (this->limit < this->BUFFER_SIZE)
            throw std::logic_error("unexpected file end");
        this->in.read(this->buffer, this->BUFFER_SIZE);
        this->limit = this->in.gcount();
        this->pos = 0;
    }
    this->pos++;
    return this->buffer[this->pos - 1];
}

std::pair<int, int> Reader::flush(){
    std::pair<int, int> r = std::make_pair(this->residual, this->resBits);
    this->residual = this->resBits = 0;
    return r;
}

void Reader::close(){
    this->in.close();
}