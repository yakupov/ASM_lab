#ifndef BLOCKHEADER_H
#define BLOCKHEADER_H

#include <cstring>

struct BlockHeader {
    BlockHeader() : magic (0xDEADBEEF) {}

    BlockHeader & operator= (const BlockHeader &arg) {
        address = arg.address;
        size = arg.size;

        return *this;
    }

    const int magic;
    void *address;
    size_t size;
};

#endif // BLOCKHEADER_H
