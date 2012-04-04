#ifndef BLOCKHEADER_H
#define BLOCKHEADER_H

#include <cstddef>

struct BlockHeader {
    BlockHeader () {}

    BlockHeader (const BlockHeader &arg) :
        alignment (arg.alignment),
        magic (arg.magic),
        offset (arg.offset),
        size (arg.size),
        start (arg.start) {
    }

    BlockHeader & operator= (const BlockHeader &arg) {
        alignment = arg.alignment;
        magic = arg.magic;
        offset = arg.offset;
        size = arg.size;
        start = arg.start;

        return *this;
    }

    size_t alignment;
    int magic;
    size_t offset;  //start + offset == start of data
    size_t size;    //size of block
    void *start;    //start of block
};

#endif // BLOCKHEADER_H
