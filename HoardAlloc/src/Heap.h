#ifndef HEAP_H
#define HEAP_H

#include <cstring>
#include <mutex>
#include <vector>

#include "src/SuperBlock.h"
#include "src/InternalAlloc.h"

class Heap {
public:
    Heap();
    void * allocate (size_t);
    void free (void *);

protected:
    std::vector <SuperBlock, InternalAlloc<SuperBlock> > blocks;
    std::mutex mutex;
};

#endif // HEAP_H
