#ifndef FREEMEMORYFRAGMENT_H
#define FREEMEMORYFRAGMENT_H

#include <cstddef>

struct FreeMemoryFragment {
    FreeMemoryFragment() :
        offset(0),
        size(0) {
    }

    FreeMemoryFragment(void * offset, size_t size) :
        offset(offset),
        size (size) {
    }

    void * offset;
    size_t size;
};

struct FreeMemoryFragmentComparator {
    bool operator()(FreeMemoryFragment a, FreeMemoryFragment b) const {
        return a.size < b.size;
    }
};

#endif // FREEMEMORYFRAGMENT_H
