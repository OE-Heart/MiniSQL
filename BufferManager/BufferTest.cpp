#include "BufferManager.hpp"
using namespace std;

int main(){
    BufferManager Buffer;
    string str = "DBSTest";

    BID bid = Buffer.bread(str, 1);
    char* data = Buffer.baddr(bid);
    strcpy(data, "This is a teststring!\n");
    Buffer.bwrite(bid);
    Buffer.brelease(bid);

    bid = Buffer.bread(str, 1);
    data = Buffer.baddr(bid);
    printf("%s", data);
    Buffer.brelease(bid);
}