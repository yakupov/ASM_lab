#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <cstring>
#include <vector>

#include "src/BlockHeader.h"

class SuperBlock {
public:
    SuperBlock();

    void *mem;
    size_t size;
    size_t largestFreeFragmentSize;

    std::vector <BlockHeader> blocks;
};

#endif // SUPERBLOCK_H
