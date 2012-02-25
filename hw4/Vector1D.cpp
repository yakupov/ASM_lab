#include "Vector1D.h"


Vector1D::Vector1D(size_t size) :
    size (size), 
    data (size, 1) {
}


Vector1D::Vector1D(const std::string &filename) {
    std::ifstream input (filename.c_str());
    input >> size;
    double tmp;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            input >> tmp;
        }
        input >> tmp; //B column
        data.push_back(tmp);
    }
}


Vector1D::Vector1D (const Vector1D& arg) :
    size (arg.size),
    data (arg.data) {
}



double Vector1D::abs() const {
    double res = 0;
    for (int i = 0; i < size; ++i) {
        res += data[i] * data[i];
    }
    return res;
}


void Vector1D::addVector (Vector1D arg) {
    assert (arg.size == this->size);

    for (int i = 0; i < size; ++i) {
        data[i] += arg.data[i];
    }
}


double Vector1D::elementAt(int x) const {
    assert (x < size);

    return data[x];
}


void Vector1D::subtractVector (Vector1D arg) {
    assert (arg.size == this->size);

    for (int i = 0; i < size; ++i) {
        data[i] -= arg.data[i];
    }
}
