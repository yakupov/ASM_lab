#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <vector>
#include "src/BlockHeader.h"
#include "src/DefaultValues.h"
#include "src/InternalAlloc.h"
#include "src/Heap.h"

class Allocator {
public:
    Allocator();
    void * allocate (size_t size, size_t alignment = DEFAULT_ALIGNMENT);
    static bool isValidAlignment (size_t alignment);
    void * realloc (void * address, size_t size);
    void release (void *address);


protected:
    static bool isPowerOfTwo(size_t n) {
        return (n & (n >> 1)) == 0;
    }

    std::vector <Heap, InternalAlloc<Heap> > heaps;
    std::vector <BlockHeader, InternalAlloc<BlockHeader> > largeObjects;
};

#endif // ALLOCATOR_H
