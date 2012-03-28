#ifndef HEAP_H
#define HEAP_H

#include <cstring>
#include <exception>
#include <mutex>
#include <vector>
#include "src/BlockHeader.h"
#include "src/InternalAlloc.h"
#include "src/SuperBlock.h"

class Heap {
public:
    Heap();
    void * allocate (size_t, size_t alignment);
    int findBlock (void * address);
    bool hasBlock (void * address);
    void * reallocate (void * address, size_t size);
    void release (void *address);

protected:
    //void transferBlockToAnotherHeap (int index, const Heap & heap); //TODO

    std::vector <SuperBlock, InternalAlloc<SuperBlock> > blocks;
    //std::mutex mutex;                                               //TODO
};

#endif // HEAP_H
