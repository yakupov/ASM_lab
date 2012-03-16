#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <cstring>
#include <unistd.h>

#include "blockheader.h"
#include "hardcorelinkedlist.h"

class SuperBlock {
public:
    SuperBlock();

    void *mem;
    size_t size;
    size_t largestFreeFragmentSize;

    HardcoreLinkedList <BlockHeader> blocks;
};

#endif // SUPERBLOCK_H
