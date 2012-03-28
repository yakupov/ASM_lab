#ifndef DEFAULTVALUES_H
#define DEFAULTVALUES_H

#define DEFAULT_ALIGNMENT 8
#define DEFAULT_MAGIC 0xDEADBEEF
#define DEFAULT_SIZE 8192

#include <iostream>
#include <cstdio>

#define assert(exp) {                                                     \
    if(!(exp)) {                                                          \
        std::cerr << "Assertion failed: " << (char*)(__FILE__) << "("     \
                  << (int)__LINE__ << "): " << (char*)(__func__)          \
                  << std::endl << "Failed expression:  " << #exp << std::endl;         \
        abort ();                                                         \
    }                                                                     \
}




#endif // DEFAULTVALUES_H
