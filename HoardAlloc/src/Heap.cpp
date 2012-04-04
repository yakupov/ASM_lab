#include "Heap.h"

Heap::Heap() {
}

//#include <iostream> //delete later!


void * Heap::allocate(size_t size, size_t alignment) {
    std::lock_guard<std::mutex> lock(mut);
    size_t maxRequirements = size + alignment + sizeof (BlockHeader);
    assert (maxRequirements <= DEFAULT_SIZE);

    //std::cout << "trying Heap::allocate\n";
    //std::cout << "heap trying to alloc " << size << " current sb's max: " << blocks[i].maxAlloc() << std::endl;


    for (unsigned int i = 0; i < blocks.size(); ++i) {
        if (blocks[i].maxAlloc() == DEFAULT_SIZE){//>= maxRequirements) {
            return blocks[i].allocate (size, alignment);
        }
    }

    //std::cout << "heap trying to alloc " << size << " creating new sb " << std::endl;
    SuperBlock tsb;
    blocks.push_back(tsb);
    return blocks[blocks.size() - 1].allocate (size, alignment);
}


int Heap::findBlock(void *address) {
    for (unsigned int i = 0; i < blocks.size(); ++i) {
        if (address >= blocks[i].getDataStart() && address < (blocks[i].getDataStart() + blocks[i].getSize())) {
            return (int)i;
        }
    }

    return -1;
}


bool Heap::hasBlock(void *address) {
    return (findBlock(address) != -1);
}


void * Heap::reallocate(void *address, size_t size) {
    std::lock_guard<std::mutex> lock(mut);
    //std::cout << "trying Heap::reallocate\n";

    int oldBlockIndex = findBlock(address);
    BlockHeader * header = (BlockHeader *)(address - sizeof (BlockHeader));
    assert (header->magic == (int)0xDEADBEEF && oldBlockIndex >= 0);

    //Shrink
    if (header->size - header->offset >= size) {
        blocks[oldBlockIndex].shrink(address, size);
        return address;
    }

    //Extend
    size_t maxRequirements = size + header->alignment + sizeof (BlockHeader);
    int newBlockIndex = -1;
    for (unsigned int i = 0; i < blocks.size(); ++i) {
        if (blocks[i].maxAlloc() >= maxRequirements) {
            //newBlockIndex = (int)i;
            break;
        }
    }
    if (newBlockIndex < 0) {
        SuperBlock tsb;
        blocks.push_back(tsb);
        newBlockIndex = blocks.size() - 1;
    }

    void * newAddress = blocks[newBlockIndex].allocate (size, header->alignment);
    memcpy (newAddress, address, header->size - header->offset);
    //blocks[oldBlockIndex].release(address);
    return newAddress;


    std::cerr << "Heap::reallocate failed\n";
    for (unsigned int i = 0; i < blocks.size(); ++i) {
        std::cerr << "max sz al oldsz off\n";
        std::cerr << blocks[i].maxAlloc() << ' ' << size << ' ' << header->alignment << ' ' << header->size << ' ' << header->offset << std::endl;
    }
    abort();
    return address;
}



void Heap::release(void *address) {
    std::lock_guard<std::mutex> lock(mut);
    //std::cout << "trying to release " << blockIndex << ' ' << address << ' ' << blocks[blockIndex].getDataStart() << std::endl;

    int blockIndex = findBlock(address);
    assert (blockIndex >= 0);
    blocks[blockIndex].release(address);

    //TODO: consider moving sb to another heap
}
