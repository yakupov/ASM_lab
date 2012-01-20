#include "mulvectors.h"

double mulVectors (double *first, double *second, size_t sz) {
    double res = 0.0;
    for (size_t i = 0; i < sz; ++i) {
        res += *(first + SIZE_OF_ELEM*i);
        res += *(second + SIZE_OF_ELEM*i);
    }
    return res;
}

