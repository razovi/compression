#include "reader.h"
#include "writer.h"
#include <cstdlib>
#include <time.h>
#include <utility>

int test(){
    srand(time(0));
    Reader r("C:\\d\\work\\compression\\bin\\a.in");
    Writer w("C:\\d\\work\\compression\\bin\\a.out", 2);
    while (true){
        int bits = rand() % 15 + 1;
        try{
            int x = r.get(bits);
            w.push(x, bits);
        }
        catch (...){
            std::pair<int, int> res = r.flush();
            w.push(res.first, res.second); 
            break;
        }
    }
    return 0;
}