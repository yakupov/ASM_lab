#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include "src/Allocator.h"
#include "src/DefaultValues.h"

#include <iostream>
#include "src/BlockHeader.h"

//std::vector <Heap> heaps;
Allocator ha;


bool trace_enabled() {
    static bool enabled = (getenv("MALLOC_INTERCEPT_NO_TRACE") == NULL);
    return enabled;
}


bool trace_enabled2() {
    static bool enabled = (getenv("MINT") == NULL);
    return enabled;
}


extern "C"
void* malloc(size_t size) {
    //fprintf(stderr, "gonna malloc %zu\n", size);

    void * p = ha.allocate(size, DEFAULT_ALIGNMENT);

    BlockHeader * head = (BlockHeader *)(p - sizeof (BlockHeader));
    if (trace_enabled2()) {
        printf ("header values: (mag, al, sz, off, req_size)\n");
        printf ("%X %zu %zu %zu %zu\n", head->magic, head->alignment, head->size, head->offset, size);
    }
    assert (head->magic == (int)0xDEADBEEF);
    assert (head->alignment == DEFAULT_ALIGNMENT);
    assert (head->size - head->offset == size);
    assert (head->start + head->offset == p);
    assert ((unsigned)p % DEFAULT_ALIGNMENT == 0);

    if (trace_enabled())
        // its generally bad idea to call I/O function from malloc
        // if they call malloc we will end up with an infinite recursion...
        // this is why I use fprintf instead of std::cerr
        fprintf(stderr, "malloc %zu %p\n", size, p);

    return p;
}


extern "C"
void free(void *ptr) {
    //fprintf(stderr, "gonna free \n");

    if (ptr == NULL) {
        fprintf(stderr, "free: null pointer\n");
        return;
    }

    BlockHeader * head = (BlockHeader *)(ptr - sizeof (BlockHeader));
    if (trace_enabled2()) {
        printf ("header values: (mag, al, sz, off)\n");
        printf ("%X %zu %zu %zu\n", head->magic, head->alignment, head->size, head->offset);
    }
    assert (head->magic == (int)0xDEADBEEF);
    assert (head->start + head->offset == ptr);
    assert ((unsigned)ptr % head->alignment == 0);

    ha.release(ptr);

    if (trace_enabled())
        fprintf(stderr, "free %p\n", ptr);
}


extern "C"
void* realloc(void *ptr, size_t size) {
    //fprintf(stderr, "gonna realloc %zu\n", size);

    if (ptr == 0) {
        void * p = ha.allocate(size, DEFAULT_ALIGNMENT);
        fprintf(stderr, "realloc with nullptr %p %zu %p\n", ptr, size, p);
        return p;
    }


    BlockHeader oldh = *((BlockHeader *)(ptr - sizeof (BlockHeader)));

    void* p = ha.realloc(ptr, size);

    BlockHeader * head = (BlockHeader *)(p - sizeof (BlockHeader));
    if (trace_enabled2()) {
        printf ("header values: (mag, al, sz, off, req_size)\n");
        printf ("%X %zu %zu %zu %zu\n", head->magic, head->alignment, head->size, head->offset, size);
    }
    assert (head->magic == (int)0xDEADBEEF);
    assert (oldh.magic == (int)0xDEADBEEF);
    assert (ha.isValidAlignment(oldh.alignment));
    assert (head->alignment == oldh.alignment);
    assert (head->size - head->offset == size);
    assert (head->start + head->offset == p);
    assert ((unsigned)p % oldh.alignment == 0);


    if (trace_enabled())
        fprintf(stderr, "realloc %p %zu %p\n", ptr, size, p);

    return p;
}


extern "C"
void* calloc(size_t n, size_t size) {
    //fprintf(stderr, "gonna calloc %zu\n", size);
    void *p=ha.allocate(size * n, DEFAULT_ALIGNMENT);

    BlockHeader * head = (BlockHeader *)(p - sizeof (BlockHeader));
    if (trace_enabled2()) {
        printf ("header values: (mag, al, sz, off, req_size)\n");
        printf ("%X %zu %zu %zu %zu\n", head->magic, head->alignment, head->size, head->offset, size);
    }
    assert (head->magic == (int)0xDEADBEEF);
    assert (head->alignment == DEFAULT_ALIGNMENT);
    assert (head->size - head->offset == size * n);
    assert (head->start + head->offset == p);
    assert ((unsigned)p % DEFAULT_ALIGNMENT == 0);

    if (trace_enabled())
        fprintf(stderr, "calloc %zu %zu %p\n", n, size, p);

    return p;
}


extern "C"
int posix_memalign(void** memptr, size_t alignment, size_t size) {
    //fprintf(stderr, "gonna pma %zu %zu \n", size, alignment);
    *memptr = 0;

    if (!ha.isValidAlignment(alignment))
        return EINVAL;

    void* p = ha.allocate(size, alignment);


    BlockHeader * head = (BlockHeader *)(p - sizeof (BlockHeader));
    if (trace_enabled2()) {
        printf ("header values: (mag, al, sz, off, req_size)\n");
        printf ("%X %zu %zu %zu %zu\n", head->magic, head->alignment, head->size, head->offset, size);
    }
    assert (head->magic == (int)0xDEADBEEF);
    assert (head->alignment == alignment);
    assert (head->size - head->offset == size);
    assert (head->start + head->offset == p);
    assert ((unsigned)p % alignment == 0);



    if (trace_enabled())
        fprintf(stderr, "posix_memalign %zu %zu %p\n", alignment, size, p);

    if (p == 0) {
        return ENOMEM;
        fprintf(stderr, "pma problem\n");
    }


    *memptr = p;

    return 0;
}


extern "C"
void *valloc(size_t size) {
    (void) size;
    fprintf(stderr, "deprecated function valloc is not supported\n");
    std::abort();
}


extern "C"
void *memalign(size_t boundary, size_t size) {
    (void) size;
    (void) boundary;
    fprintf(stderr, "deprecated function memalign is not supported\n");
    std::abort();
}
