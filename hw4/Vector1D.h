#ifndef VECTOR1D_H
#define VECTOR1D_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Vector.h"

class Vector2D;

class Vector1D : public Vector {
public:
    friend class Vector2D;

    Vector1D (size_t dimension);
    Vector1D (std::string filename);
    Vector1D (const Vector1D& arg);
    double abs();
    void addVector (Vector1D arg);
    double elementAt (int);
    int getDimension () {return dimension;}
    void subtractVector (Vector1D arg);
};

#endif // VECTOR1D_H
