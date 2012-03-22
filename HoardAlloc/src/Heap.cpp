#include "Heap.h"

Heap::Heap() {
}

#include <iostream>


void * Heap::allocate(size_t size) {
    for (unsigned int i = 0; i < blocks.size(); ++i) {

//	std::cout << "heap trying to alloc " << size << " current sb's max: " << blocks[i].maxAlloc() << std::endl; 

        if (blocks[i].maxAlloc() >= size) {
            return blocks[i].allocate (size);
        }
    }


//    std::cout << "heap trying to alloc " << size << " creating new sb " << std::endl; 

    SuperBlock tsb;
    blocks.push_back(tsb);

    return blocks[blocks.size() - 1].allocate (size);
}


int Heap::findBlock(void *address) {
    for (unsigned int i = 0; i < blocks.size(); ++i) {
        if (address >= blocks[i].getOffset() && address < blocks[i].getOffset() + blocks[i].getSize()) {
            return (int)i;
        }
    }

    return -1;
}


bool Heap::hasBlock(void *address) {
    return (findBlock(address) != -1);
}

//#include <iostream>

void Heap::release(void *address) {
    int blockIndex = findBlock(address);
	
//	std::cout << "trying to release " << blockIndex << ' ' << address << ' ' << blocks[blockIndex].getOffset() << std::endl;

    if (blockIndex == -1) {
        throw std::bad_alloc();
    }

    blocks[blockIndex].release(address);

    //TODO: consider moving sb to another heap
}
