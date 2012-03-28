#ifndef FREEMEMORYFRAGMENT_H
#define FREEMEMORYFRAGMENT_H

#include <cstddef>

struct FreeMemoryFragment {
    FreeMemoryFragment() :
        address(0),
        size(0) {
    }

    FreeMemoryFragment(void * address, size_t size) :
        address(address),
        size (size) {
    }

    void * address;
    size_t size;
};

struct FreeMemoryFragmentComparator {
    bool operator()(FreeMemoryFragment a, FreeMemoryFragment b) const {
        return a.size < b.size;
    }
};

#endif // FREEMEMORYFRAGMENT_H
