#ifndef HARDCORELINKEDLIST_H
#define HARDCORELINKEDLIST_H

#include <unistd.h>
#include <sys/mman.h>

template <class T>
struct HardcoreLinkedList {
    HardcoreLinkedList() :
        next (NULL),
        prev (NULL) {
    }

    HardcoreLinkedList *fork () {
        HardcoreLinkedList *res = (HardcoreLinkedList *)mmap(NULL, sizeof(HardcoreLinkedList),
                                                             PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        res->HardcoreLinkedList();
        //res->next = NULL;
        //res->prev = NULL;
        res->element = element;

        return res;
    }

    T element;
    HardcoreLinkedList* next;
    HardcoreLinkedList* prev;
};

#endif // HARDCORELINKEDLIST_H
