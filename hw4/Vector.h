#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <fstream>
#include <string>

class Vector {
protected:
    Vector() {}
    Vector (const Vector& arg) :
	dimension (arg.dimension),
	data (arg.data) {
    }
    std::vector<double> data;
    int dimension;
};

#endif // VECTOR_H
