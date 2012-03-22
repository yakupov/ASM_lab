#ifndef HEAP_H
#define HEAP_H

#include <cassert>
#include <cstring>
#include <exception>
#include <mutex>
#include <vector>

#include "src/SuperBlock.h"
#include "src/InternalAlloc.h"

class Heap {
public:
    Heap();
    void * allocate (size_t);
    int findBlock (void * address);
    bool hasBlock (void * address);
    void release (void *address);

protected:
    //void transferBlockToAnotherHeap (int index, const Heap & heap); //TODO

    std::vector <SuperBlock, InternalAlloc<SuperBlock> > blocks;
    //std::mutex mutex;                                               //TODO
};

#endif // HEAP_H
