#include "Heap.h"

Heap::Heap() {
}

#include <iostream> //delete later!


void * Heap::allocate(size_t size, size_t alignment) {
    std::cout << "trying Heap::allocate\n";

    for (unsigned int i = 0; i < blocks.size(); ++i) {
      //  std::cout << "heap trying to alloc " << size << " current sb's max: " <<blocks[i].maxAlloc() << std::endl;

        if (blocks[i].tryAlloc(size, alignment)) {
            void * p = blocks[i].allocate (size, alignment);

            BlockHeader * head = (BlockHeader *)(p - sizeof (BlockHeader));

            std::cout << "Heap::allocate: header values: (mag, al, sz, off)\n";
            printf ("%p %zu %zu %zu\n", head->magic, head->alignment, head->size, head->offset);
            assert (head->magic == 0xDEADBEEF);
            assert (head->alignment == alignment);
            assert (head->size - head->offset == size);
            assert (head->start + head->offset == p);
            assert ((unsigned)p % alignment == 0);

            return p;

        }
    }


    //std::cout << "heap trying to alloc " << size << " creating new sb " << std::endl;
    SuperBlock tsb;
    blocks.push_back(tsb);
    void * p = blocks[blocks.size() - 1].allocate (size, alignment);
    //std::cout << p << std::endl;

    //BlockHeader * head = (BlockHeader *)(p - sizeof (BlockHeader));
    //std::cerr << "Heap::fuck you assert, " << head->size - head->offset << ' ' << head->alignment << ' ' << (void *)head->magic << ' ';
    //std::cerr <<  head->size << ' ' << head->offset << ' ' << alignment << std::endl;

    return p;
}


int Heap::findBlock(void *address) {
    for (unsigned int i = 0; i < blocks.size(); ++i) {
        if (address >= blocks[i].getDataStart() && address < blocks[i].getDataStart() + blocks[i].getSize()) {
            return (int)i;
        }
    }

    return -1;
}


bool Heap::hasBlock(void *address) {
    return (findBlock(address) != -1);
}


void * Heap::reallocate(void *address, size_t size) {
    std::cout << "trying Heap::reallocate\n";

    int blockIndex = findBlock(address);
    BlockHeader * header = (BlockHeader *)(address - sizeof (BlockHeader));

    if (header->magic != 0xDEADBEEF || blockIndex < 0) {
        std::cout << "heap: bad magic or block not found\n" << header->magic << std::endl;

        abort();
    }

    if (header->size - header->offset >= size) {
        std::cout << "will be shrinked " << size  << ' ' << header->size << ' ' << header->offset << std::endl;

        blocks[blockIndex].shrink(address, size);
        return address;
    }

    void * newAddress = allocate(size, header->alignment);
    memcpy (newAddress, address, header->size - header->offset);
    blocks[blockIndex].release(address);

    return newAddress;
}



void Heap::release(void *address) {
    int blockIndex = findBlock(address);
	
    //std::cout << "trying to release " << blockIndex << ' ' << address << ' ' << blocks[blockIndex].getDataStart() << std::endl;

    if (blockIndex == -1) {
        std::cerr << "block not found in this heap, bad code\n";

        abort();
    }

    blocks[blockIndex].release(address);

    //TODO: consider moving sb to another heap
}
