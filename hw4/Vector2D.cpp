#include "Vector2D.h"

Vector2D::Vector2D(std::string filename) {
    std::ifstream input (filename.c_str());
    input >> dimension;
    double tmp;
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            input >> tmp;
            data.push_back(tmp);
        }
        input >> tmp; //B column
    }
}


Vector2D::Vector2D (const Vector2D& arg):
    Vector (arg) {
}


double Vector2D::elementAt(int x, int y) {
    if (x >= dimension || y >= dimension) {
        throw ("index out of bounds");
    }

    int index = dimension * y + x;
    return data[index];
}


Vector1D Vector2D::multiplyByVector (Vector1D &arg) {
    Vector1D res(arg);
    for (int i = 0; i < dimension; ++i) {
	res.data[i] = mulVectors (&(this->data[0]) + dimension * i, &(arg.data[0]), dimension);
    }
    return res;
}
