#include "Heap.h"

Heap::Heap() {
}



bool Heap::hasBlock(void *address) {
    for (unsigned int i = 0; i < blocks.size(); ++i) {
        if (address >= blocks[i].getOffset() && address < blocks[i].getOffset() + blocks[i].getSize()) {
            return true;
        }
    }
    return false;
}
