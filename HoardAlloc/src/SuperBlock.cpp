#include "SuperBlock.h"

#include <iostream>



SuperBlock::SuperBlock(int size) :
      size (size) {
    memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    freeSpace.insert(FreeMemoryFragment(memory, size));
//    std::cout << "sb created " << size << " max: " << maxAlloc() << " addr: " << memory << std::endl;
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
    std::cout << "alloc: " << size << " / " << maxAlloc() << std::endl;
    possibleAllocData allocData;

    if (!tryAlloc(size, alignment, &allocData)) {
        std::cout << "bad code (alloc)" << std::endl;
        //throw std::bad_alloc();
        abort();
    }

    BlockHeader header;
    header.size = size + allocData.offset;
    header.start = (*allocData.fragment).address;
    header.offset = allocData.offset;
    header.alignment = alignment;
    header.magic = 0xDEADBEEF;

    blocks.push_back(header);

    size_t remains = (*allocData.fragment).size - header.size;
    freeSpace.erase(allocData.fragment);
    if (remains > 0) {
        freeSpace.insert(FreeMemoryFragment(header.start + header.size, remains));
    }

    void * dataStart = header.start + header.offset;
    BlockHeader * head = (BlockHeader *)(dataStart - sizeof (BlockHeader));
    *head = header;

    std::cout << "SB::allocate: header values: (mag, al, sz, off)\n";
    printf ("%p %zu %zu %zu\n", head->magic, head->alignment, head->size, head->offset);
    assert (head->magic == 0xDEADBEEF);
    assert (head->alignment == alignment);
    assert (head->size - head->offset == size);
    assert (head->start + head->offset == dataStart);
    assert ((unsigned)dataStart % alignment == 0);



    return dataStart;
}


void SuperBlock::release(void *address) {
      std::cout << address << " gonna free, max: " << maxAlloc() << std::endl;

    for (unsigned int i = 0; i < blocks.size(); ++i) {
        if (blocks[i].start + blocks[i].offset == address)   {
            FreeMemoryFragment freeFrag(blocks[i].start, blocks[i].size);

        //    std::cout << blocks[i].start << ' ' << blocks[i].offset << ' ' << blocks[i].start + blocks[i].offset << ' '
          //            << blocks[i].size << ' ' << blocks[i].size - blocks[i].offset << ' ' << blocks[i].alignment << std::endl;

            blocks.erase(blocks.begin() + i);

            for (std::multiset<FreeMemoryFragment>::iterator it = freeSpace.begin(); it != freeSpace.end(); ) {
                if ((*it).address + (*it).size == freeFrag.address) {
                    freeFrag.address = (*it).address;
                    freeFrag.size += (*it).size;

                    freeSpace.erase(it);
                    it = freeSpace.begin();
                } else if ((*it).address == freeFrag.address + freeFrag.size) {
                    freeFrag.size += (*it).size;

                    freeSpace.erase(it);
                    it = freeSpace.begin();
                } else {
		    ++it;
		}
            }

            freeSpace.insert(freeFrag);

            //std::cout << address << " freed, max: " << maxAlloc() << std::endl;

            return;
        }
    }

    assert ("SuperBlock::release: " == "this code shouldn't be executed :(");
}


void SuperBlock::shrink(void *address, size_t size) {
    for (unsigned int i = 0; i < blocks.size(); ++i) {
        if (blocks[i].start + blocks[i].offset == address)   {
            size_t dataSize = blocks[i].size - blocks[i].offset;

            if (dataSize < size) {
                std::cout << "bad code (realloc)" << std::endl;
                abort();
                //throw std::bad_alloc();
            }

            size_t remains = dataSize - size;
            blocks[i].size = blocks[i].offset + size;
            if (remains > 0) {
                freeSpace.insert(FreeMemoryFragment(blocks[i].start + blocks[i].size, remains));
            }


            BlockHeader * header = (BlockHeader *)(address - sizeof (BlockHeader));
            *header = blocks[i];

            return;
        }
    }
}


bool SuperBlock::tryAlloc(size_t size, size_t alignment, possibleAllocData *allocData) {
    //std::cout << "trying: " << size << " / " << maxAlloc() << std::endl;

    size_t offset = sizeof (BlockHeader);
    size_t actualSize = size + offset;

    for (std::multiset<FreeMemoryFragment>::iterator it = freeSpace.begin(); it != freeSpace.end(); ++it) {
        if ((*it).size < actualSize)
            continue;

        offset = sizeof (BlockHeader);
        actualSize = size + offset;
        void *start = (*it).address;
        void *alignedDataStart = start + offset;

        while ((unsigned int)alignedDataStart % alignment != 0) {
            ++alignedDataStart;
            ++actualSize;
            ++offset;
        }


        if ((*it).size >= actualSize) {
            if (allocData != 0) {
                allocData->fragment = it;
                allocData->offset = offset;
            }
            return true;
        }
    }

    return false;
}
