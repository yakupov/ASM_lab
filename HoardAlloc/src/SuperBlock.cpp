#include "SuperBlock.h"

#include <iostream>


SuperBlock::SuperBlock(int size) :
      size (size) {
    memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    freeSpace.insert(FreeMemoryFragment(memory, size));
//	std::cout << "sb created " << size << " max: " << maxAlloc() << " addr: " << memory << std::endl; 
}

SuperBlock::SuperBlock(const SuperBlock &arg) :
    memory (arg.memory),
    size (arg.size),
    blocks (arg.blocks),
    freeSpace (arg.freeSpace) {
}


SuperBlock::~SuperBlock() {
    //munmap (memory, size);
}


void * SuperBlock::allocate(size_t size) {
    if (maxAlloc() < size) {
        throw std::bad_alloc();
    }

    void * address = 0;
    for (std::multiset<FreeMemoryFragment>::iterator it = freeSpace.begin(); it != freeSpace.end(); ++it) {
        if ((*it).size >= size) {
            address = (*it).offset;
            size_t remains = (*it).size - size;

            BlockHeader header;
            header.size = size;
            header.address = address;
            blocks.push_back(header);

//	std::cout << "sb trying to alloc " << size << " max: " << maxAlloc() << std::endl; 

            freeSpace.erase(it);
            if (remains > 0) {
                freeSpace.insert(FreeMemoryFragment(address + size, remains));
            }
            return address;
        }
    }

    assert ("SuperBlock::allocate: " == "this code shouldn't be executed :(");
    return address;
}


size_t SuperBlock::maxAlloc() {
    if (freeSpace.size() == 0) {
        return 0;
    }
    return (*--freeSpace.end()).size;
}


void SuperBlock::release(void *mem) {
//	std::cout << mem << " gonna free, max: " << maxAlloc() << std::endl;
    for (unsigned int i = 0; i < blocks.size(); ++i) {
        if (blocks[i].address == mem)   {
            FreeMemoryFragment freeFrag(mem, blocks[i].size);
            blocks.erase(blocks.begin() + i);

            for (std::multiset<FreeMemoryFragment>::iterator it = freeSpace.begin(); it != freeSpace.end(); ) {
                if ((*it).offset + (*it).size == freeFrag.offset) {
                    freeFrag.offset = (*it).offset;
                    freeFrag.size += (*it).size;

                    freeSpace.erase(it);
                    it = freeSpace.begin();
                } else if ((*it).offset == freeFrag.offset + freeFrag.size) {
                    freeFrag.size += (*it).size;

                    freeSpace.erase(it);
                    it = freeSpace.begin();
                } else {
		    ++it;
		}
            }

            freeSpace.insert(freeFrag);

//		std::cout << mem << " freed, max: " << maxAlloc() << std::endl;
	
            return;
        }
    }

    assert ("SuperBlock::release: " == "this code shouldn't be executed :(");
}
