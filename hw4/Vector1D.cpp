#include "Vector1D.h"

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


double Vector1D::elementAt(int x) {
    if (x >= dimension) {
        throw ("index out of bounds");
    }

    return data[x];
}
