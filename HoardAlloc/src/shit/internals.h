#ifndef INTERNALS_H
#define INTERNALS_H

#include <sys/mman.h>

void *operator new(size_t size) {
    fuck *f2;
    void* mem = mmap(NULL, sizeof(fuck), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

}

#endif // INTERNALS_H
