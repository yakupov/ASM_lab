#ifndef BLOCKHEADER_H
#define BLOCKHEADER_H

#include <cstring>

struct BlockHeader {
    //BlockHeader() : magic (0xDEADBEEF) {}

    BlockHeader & operator= (const BlockHeader &arg) {
        alignment = arg.alignment;
        offset = arg.offset;
        start = arg.start;
        size = arg.size;
        magic = arg.magic;

        return *this;
    }

    size_t alignment;
    //const
    int magic;
    size_t offset;  //start + offset == start of data
    void *start;    //start of block
    size_t size;    //size of block
};

#endif // BLOCKHEADER_H
