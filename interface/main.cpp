#include "../compress/compress.h"
#include "../io/reader.h"
#include "../io/writer.h"

int main(){
    const char *fin = "C:\\d\\work\\compression\\bin\\a.in";
    const char *fout = "C:\\d\\work\\compression\\bin\\a.out";
    const char *ftest = "C:\\d\\work\\compression\\bin\\a.din";
    Reader r(fin);
    Writer w(fout, 2);
    LZWc(r, w);
    r.close();
    w.close();
    
    Reader r2(fout);
    Writer w2(ftest, 1);
    LZWd(r2, w2);
    r2.close();
    w2.close();
    return 0;
}