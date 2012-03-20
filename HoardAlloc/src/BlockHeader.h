#ifndef BLOCKHEADER_H
#define BLOCKHEADER_H

#include <cstring>

struct BlockHeader {
    BlockHeader() : magic (0xDEADBEEF) {}
    const int magic;
    void *address;
    size_t size;
};

#endif // BLOCKHEADER_H
