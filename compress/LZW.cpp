#include "../io/reader.h"
#include "../io/writer.h"
#include <iostream>
#include <vector>
#include <string>
#define QBIT 4
#define MQBIT 16

struct Trie{
    long long code;
    Trie *link[MQBIT];
    Trie *dad;
    char l;
};
Trie *root;

void newNode(Trie **t){
    *t = new Trie;
    (*t)->dad = nullptr;
    (*t)->code = 0;
    for (int i = 0; i < MQBIT; i++){
        (*t)->link[i] = nullptr;
    }
}

void recDel(Trie *p){
    for (int i = 0; i < MQBIT; i++){
        if(p->link[i] != nullptr){
            recDel(p->link[i]);
            delete p->link[i];
        }
    }
}

void LZWc(Reader &r, Writer &w){
    newNode(&root);
    for (int i = 0; i < MQBIT; i++){
        newNode(&root->link[i]);
        root->link[i]->code = i;
    }

    long long cCode = MQBIT;
    int cLen = QBIT + 1;
    long long cCutoff = (1LL << cLen);
    Trie *curr = root;

    while (true){
        try{
            char x = (char)r.get(QBIT);
            if (curr->link[x] != nullptr){
                curr = curr->link[x];
            }
            else{
                w.push(curr->code, cLen);
                newNode(&curr->link[x]);
                curr->link[x]->code = cCode++;
                if (cCutoff == cCode){
                    cCutoff *= 2;
                    cLen++;
                }
                curr = root->link[x];
            }
        }
        catch (...){
            w.push(curr->code, cLen);
            break;
        }
    }

    recDel(root);
}

void write(Writer &w, char x){
}

char recWrite(Writer &w, Trie *x){
    char r;
    if (x->dad == root)
        r = x->l;
    else
        r = recWrite(w, x->dad);
    w.push(x->l, 4);
    return r;
}

void LZWd(Reader &r, Writer &w){
    newNode(&root);
    for (int i = 0; i < MQBIT; i++){
        newNode(&root->link[i]);
        root->link[i]->dad = root;
        root->link[i]->l = i;
    }
    std::vector<Trie*> table;
    for (int i = 0; i < MQBIT; i++){
        table.push_back(root->link[i]);
    }

    long long len = r.get(64);

    long long cCode = MQBIT;
    int cLen = QBIT + 1;
    long long cCutoff = (1LL << cLen);

    long long old = r.get(cLen);
    len -= cLen;
    Trie *s = table[old];
    recWrite(w, s);
    while (len > 0){
        long long n = r.get(cLen);
        char x;
        len -= cLen;
        if (n >= table.size()){
            s = table[old];
            x = recWrite(w, s);
            w.push(x, 4);
        }
        else{
            s = table[n];
            x = recWrite(w, s);
        }
        newNode(&table[old]->link[x]);
        table[old]->link[x]->dad = table[old];
        table[old]->link[x]->l = x;
        table.push_back(table[old]->link[x]);
        cCode++;
        if (cCode == cCutoff - 1){
            cCutoff *= 2;
            cLen++;
        }
        old = n;
    }
    recDel(root);
}
