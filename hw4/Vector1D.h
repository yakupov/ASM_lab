#ifndef VECTOR1D_H
#define VECTOR1D_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

class Vector2D;

class Vector1D {
public:
    friend class Vector2D;

    Vector1D (size_t size);
    Vector1D (const std::string &filename);
    Vector1D (const Vector1D& arg);
    double abs() const;
    void addVector (Vector1D arg);
    double elementAt (int) const;
    int getSize () const {return size;}
    void subtractVector (Vector1D arg);

protected:
    std::vector<double> data;
    int size;
};

#endif // VECTOR1D_H
