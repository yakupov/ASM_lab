#ifndef HEAP_H
#define HEAP_H

#include <cstring>
#include <mutex>

#include "superblock.h"
#include "hardcorelinkedlist.h"

class Heap {
public:
    Heap();
    void * allocate (size_t);
    void free (void *);

protected:
    HardcoreLinkedList <SuperBlock> blocks;
    std::mutex mutex;
};

#endif // HEAP_H
