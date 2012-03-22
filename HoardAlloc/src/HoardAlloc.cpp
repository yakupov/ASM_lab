#include <cstring>
#include <cstdio>
#include <vector>
#include "Heap.h"

//std::vector <Heap> heaps;
Heap heap;

bool trace_enabled()
{
    static bool enabled = (getenv("MALLOC_INTERCEPT_NO_TRACE") == NULL);
    return enabled;
}

extern "C"
void* malloc(size_t size)
{
    void *p=heap.allocate(size);//0;// = internal_alloc(size, DEFAULT_ALIGNMENT);

    if (trace_enabled())
        // its generally bad idea to call I/O function from malloc
        // if they call malloc we will end up with an infinite recursion...
        // this is why I use fprintf instead of std::cerr
        fprintf(stderr, "malloc %zu %p\n", size, p);

    return p;
}

extern "C"
void free(void *ptr)
{
    //internal_free(ptr);
    heap.release(ptr);

    if (trace_enabled())
        fprintf(stderr, "free %p\n", ptr);
}



