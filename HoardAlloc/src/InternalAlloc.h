#ifndef INTERNALALLOC_H
#define INTERNALALLOC_H

#include <limits>
#include <cstddef>
#include <sys/mman.h>

template <typename T> class InternalAlloc;

// specialize for void:
template <> class InternalAlloc<void> {
public:
    typedef void*       pointer;
    typedef const void* const_pointer;
    // reference to void members are impossible.
    typedef void value_type;
    template <class U> struct rebind { typedef InternalAlloc<U> other; };
};

template <typename T>
class InternalAlloc {
public:
    typedef size_t        size_type;
    typedef ptrdiff_t    difference_type;
    typedef T*            pointer;
    typedef const T*    const_pointer;
    typedef T&            reference;
    typedef const T&    const_reference;
    typedef T            value_type;

    template <typename U>
    struct rebind {
        typedef InternalAlloc<U> other;
    };

    InternalAlloc(){}
    InternalAlloc(const int&, const int&){}
    InternalAlloc(const InternalAlloc&){}
    template <typename U>
    InternalAlloc(const InternalAlloc<U>&){}
    ~InternalAlloc(){}

    pointer address (reference value) const {
       return &value;
    }

    const_pointer address (const_reference value) const {
        return &value;
    }

    pointer allocate (size_type n, std::allocator<void>::const_pointer hint=0) {
        return (pointer)mmap(NULL, sizeof(value_type) * n, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }

    void construct(pointer p, const_reference value){
        new((void*)p) T(value);
    }

    void destroy(pointer p){
        p->~T();
    }

    void deallocate (pointer p, size_type n) {
        munmap ((void *)p, sizeof(value_type) * n);
    }

    size_type max_size() const{
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }
};


// return that all specializations of this allocator are interchangeable
template <class T1, class T2>
bool operator== (const InternalAlloc<T1>&, const InternalAlloc<T2>&) throw() {
    return true;
}

template <class T1, class T2>
bool operator!= (const InternalAlloc<T1>&, const InternalAlloc<T2>&) throw() {
    return false;
}

#endif // INTERNALALLOC_H
