#ifndef VECTOR1D_H
#define VECTOR1D_H

#include <vector>
#include <fstream>
#include <string>
#include "Vector.h"

class Vector2D;

class Vector1D : public Vector {
public:
    friend class Vector2D;

    Vector1D (std::string filename);
    Vector1D (const Vector1D& arg);
    double elementAt (int);
};

#endif // VECTOR1D_H
