#include "Vector1D.h"


Vector1D::Vector1D(size_t dimension) {
    this->dimension = dimension;

    for (int i = 0; i < dimension; ++i) {
        data.push_back (1);
    }
}


Vector1D::Vector1D(std::string filename) {
    std::ifstream input (filename.c_str());
    input >> dimension;
    double tmp;
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            input >> tmp;
        }
        input >> tmp; //B column
        data.push_back(tmp);
    }
}


Vector1D::Vector1D (const Vector1D& arg):
    Vector (arg) {
}


double Vector1D::abs() {
    double res = 0;
    for (int i = 0; i < dimension; ++i) {
        res += data[i] * data[i];
    }
    return res;
}


void Vector1D::addVector (Vector1D arg) {
    if (arg.dimension != this->dimension) {
        std::cout << "size mismatch\n";
        throw ("size mismatch");
    }

    for (int i = 0; i < dimension; ++i) {
        data[i] += arg.data[i];
    }
}


double Vector1D::elementAt(int x) {
    if (x >= dimension) {
        std::cout << "index OOB\n";
        throw ("index out of bounds");
    }

    return data[x];
}


void Vector1D::subtractVector (Vector1D arg) {
    if (arg.dimension != this->dimension) {
        std::cout << "size mismatch\n";
        throw ("size mismatch");
    }

    for (int i = 0; i < dimension; ++i) {
        data[i] -= arg.data[i];
    }
}
