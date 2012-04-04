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

    size_t getSize () const {return size;}
    void * getStart () const {return address;}

    void setSize (size_t size) {this->size = size;}
    void setStart (void * start) {this->address = start;}

protected:
    void * address;
    size_t size;
};

struct FreeMemoryFragmentComparator {
    bool operator()(FreeMemoryFragment a, FreeMemoryFragment b) const {
        return a.getSize() < b.getSize();
    }
};

#endif // FREEMEMORYFRAGMENT_H
