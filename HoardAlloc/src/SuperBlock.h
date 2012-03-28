#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <cstring>
#include <exception>
#include <set>
#include <vector>
#include "src/BlockHeader.h"
#include "src/DefaultValues.h"
#include "src/FreeMemoryFragment.h"
#include "src/InternalAlloc.h"


class SuperBlock {
public:
    SuperBlock(int size = DEFAULT_SIZE);
    SuperBlock(const SuperBlock &arg);
    ~SuperBlock();

    struct possibleAllocData {
        std::multiset<FreeMemoryFragment>::iterator fragment;
        size_t offset;
    };

    void * allocate (size_t size, size_t alignment);
    void * getDataStart () {return memory;}
    size_t getSize () {return size;}

    size_t maxAlloc() {
        if (freeSpace.begin() == freeSpace.end())
            return 0;

        return (*--(freeSpace.end())).size;
    }

    void release (void *address);
    void shrink (void * address, size_t size);
    bool tryAlloc (size_t size, size_t alignment, possibleAllocData *allocData = 0);

protected:
    void *memory;
    size_t size;
    std::vector <BlockHeader, InternalAlloc<BlockHeader> > blocks;
    std::multiset <FreeMemoryFragment, FreeMemoryFragmentComparator, InternalAlloc<FreeMemoryFragment> > freeSpace;
};

#endif // SUPERBLOCK_H
