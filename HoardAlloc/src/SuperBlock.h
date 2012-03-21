#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <cassert>
#include <cstring>
#include <exception>
#include <set>
#include <vector>
#include "src/BlockHeader.h"
#include "src/FreeMemoryFragment.h"
#include "src/InternalAlloc.h"

#define DEFAULT_SIZE 64

class SuperBlock {
public:
    SuperBlock(int size = DEFAULT_SIZE);
    SuperBlock(const SuperBlock &arg);
    ~SuperBlock();

    //SuperBlock & operator= (const SuperBlock &arg);

    void * allocate (size_t size);
    void * getOffset () {return memory;}
    size_t getSize () {return size;}
    size_t maxAlloc ();
    void release (void *address);


protected:
    void *memory;
    size_t size;
    std::vector <BlockHeader, InternalAlloc<BlockHeader> > blocks;
    std::multiset <FreeMemoryFragment, FreeMemoryFragmentComparator, InternalAlloc<FreeMemoryFragment> > freeSpace;
};

#endif // SUPERBLOCK_H
