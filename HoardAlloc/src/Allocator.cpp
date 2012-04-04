#include "Allocator.h"

//#include <iostream>

Allocator::Allocator() {
    heaps.push_back(Heap());
}


void * Allocator::allocate(size_t size, size_t alignment) {
    //std::cout << "trying Allocator::allocate\n";

    if (size > DEFAULT_SIZE / 2 || alignment > DEFAULT_SIZE / 3) {
        //std::cout << "Allocator::allocate: large alloc; alignment and size: " << alignment << ' ' << size << std::endl;
        BlockHeader header;
        header.magic = DEFAULT_MAGIC;
        header.size = size + alignment + sizeof (BlockHeader);
        header.start = mmap(NULL, header.size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        header.alignment = alignment;
        header.offset = sizeof (BlockHeader);

        void * dataStart = header.start + header.offset;
        while ((unsigned)dataStart % alignment != 0) {
            ++header.offset;
            ++dataStart;
        }
        munmap (header.start + size + header.offset, header.size - size - header.offset);
        header.size = size + header.offset;

        BlockHeader * head = (BlockHeader *)(dataStart - sizeof (BlockHeader));
        *head = header;

        largeObjects.push_back(header);
        return dataStart;
    }

    void * p = heaps[0].allocate (size, alignment);
/*
    BlockHeader * head = (BlockHeader *)(p - sizeof (BlockHeader));
    assert (head->magic == 0xDEADBEEF);
    assert (head->alignment == alignment);
    assert (head->size - head->offset == size);
    assert (head->start + head->offset == p);
    assert ((unsigned)p % alignment == 0);
*/
    return p;
}


bool Allocator::isValidAlignment(size_t alignment) {
    if ((alignment % sizeof(void*)) != 0)
        return false;

    if (!isPowerOfTwo(alignment))
        return false;

    return true;
}


void * Allocator::realloc(void *address, size_t size) {
    //std::cout << "trying Allocator::realloc\n";

    for (unsigned int i = 0; i < heaps.size(); ++i) {
        if (heaps[i].hasBlock(address)) {
            //std::cout << i << ' ' << address << " small realloc\n";

            if (size < DEFAULT_SIZE / 2) {
                return heaps[i].reallocate (address, size);
            } else {
                BlockHeader * head = (BlockHeader *)(address - sizeof (BlockHeader));
                void * newAddress = allocate (size, head->alignment);
                memcpy (newAddress, address, head->size);
                //release (address);
                return newAddress;
            }
        }
    }

    for (unsigned int i = 0; i < largeObjects.size(); ++i) {
        if (largeObjects[i].start + largeObjects[i].offset == address) {

            //std::cout << "large realloc \n";

            if (largeObjects[i].size - largeObjects[i].offset >= size) { //shrink
                size_t newsize = largeObjects[i].offset + size;
                munmap (largeObjects[i].start + newsize, largeObjects[i].size - newsize);
                largeObjects[i].size = newsize;
                BlockHeader * head = (BlockHeader *)(address - sizeof (BlockHeader));
                head->size = newsize;
                return address;
            } else {                                                     //extend
                void * newAddress = allocate (size, largeObjects[i].alignment);
                memcpy (newAddress, address, largeObjects[i].size);
                munmap (largeObjects[i].start, largeObjects[i].size);
                largeObjects.erase(largeObjects.begin() + i);
                return newAddress;
            }
        }
    }

    std::cout << "Allocator::realloc: somethind failed badly. size: " << size << std::endl;
    abort();
    return address;
}


void Allocator::release(void *address) {
    //std::cout << "trying Allocator::release\n";

    for (unsigned int i = 0; i < heaps.size(); ++i) {
        if (heaps[i].hasBlock (address)) {
            heaps[i].release (address);
            return;
        }
    }

    for (unsigned int i = 0; i < largeObjects.size(); ++i) {
        if (largeObjects[i].start + largeObjects[i].offset == address) {
            munmap (largeObjects[i].start, largeObjects[i].size);
            largeObjects.erase(largeObjects.begin() + i);
            return;
        }
    }
}

