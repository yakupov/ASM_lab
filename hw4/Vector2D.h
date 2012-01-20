#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <vector>
#include <fstream>
#include <string>
#include "Vector.h"
#include "Vector1D.h"
#include "mulvectors.h"

class Vector2D : public Vector {
public:
    Vector2D (std::string filename);
    Vector2D (const Vector2D& arg);
    double elementAt (int, int);
    Vector1D multiplyByVector (Vector1D &arg);
};

#endif // VECTOR2D_H
