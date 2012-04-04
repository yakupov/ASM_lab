#include "SuperBlock.h"
//#include <iostream>

SuperBlock::SuperBlock(int size) :
      size (size) {
    memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    freeSpace.insert(FreeMemoryFragment(memory, size));
}

SuperBlock::SuperBlock(const SuperBlock &arg) :
    memory (arg.memory),
    size (arg.size),
    blocks (arg.blocks),
    freeSpace (arg.freeSpace) {
}


SuperBlock::~SuperBlock() {
    //munmap (memory, size); //consider other way of automatic memory freeing... or delete copy constr.. or change it...
}


void * SuperBlock::allocate(size_t size, size_t alignment) {
    //assert (size > 0);


    size_t offs = sizeof (BlockHeader);
    void * alstart = memory + offs;
    while ((unsigned)alstart % alignment != 0) {
        offs++;
        alstart++;
    }

    BlockHeader header;
    header.magic = 0xDEADBEEF;
    header.start = memory;
    header.offset = offs;
    header.alignment = alignment;
    header.size = size + offs;

    blocks.push_back(header);

    freeSpace.clear();
    BlockHeader * head = (BlockHeader *)(alstart - sizeof (BlockHeader));
    *head = header;
    return alstart;
}


size_t SuperBlock::maxAlloc() {
    if (freeSpace.size() == 0)
        return 0;

    return (*--(freeSpace.end())).getSize();
}


void SuperBlock::release(void *address) {
    BlockHeader * header = (BlockHeader *)(address - sizeof (BlockHeader));
    assert (header->start == memory);
    assert (header->start  + header->offset == address);
    assert (blocks.size() == 1);
    assert (blocks[0].start + blocks[0].offset == address);

    freeSpace.clear();
    freeSpace.insert(FreeMemoryFragment(memory, size));
    blocks.clear();
}


void SuperBlock::shrink(void *address, size_t size) {
    BlockHeader * header = (BlockHeader *)(address - sizeof (BlockHeader));
    assert (header->start == memory);
    assert (header->start  + header->offset == address);
    assert (blocks.size() == 1);
    assert (blocks[0].start + blocks[0].offset == address);

    assert (header->size - header->offset >= size);
    header->size = size + header->offset;
    blocks[0].size = size + header->offset;
}

