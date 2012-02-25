#include "Vector2D.h"


Vector2D::Vector2D(size_t size) {
    this->size = size;

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (i != j) {
                data.push_back (0);
            } else {
                data.push_back (1);
            }
        }
    }
}


Vector2D::Vector2D(const std::string &filename) {
    std::ifstream input (filename.c_str());
    input >> size;

    for (int i = 0; i < size; ++i) {
        double tmp = 0;
        for (int j = 0; j < size; ++j) {
            input >> tmp;
            data.push_back(tmp);
        }
        input >> tmp; //B column
    }
}


Vector2D::Vector2D (const Vector2D& arg) :
    size (arg.size),
    data (arg.data) {
}



void Vector2D::becomeInverseDiag() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i != j) {
                this->writeTo(i, j, 0);
            } else {
                writeTo(i, i, 1/elementAt(i, i));
            }
        }
    }
}



double Vector2D::elementAt(int y, int x) const {
    assert (x < size && y < size);

    int index = size * y + x;
    return data[index];
}


//in order to use mulVectors, we'd have to create %size new vectors
//that's too slow, so no SSE here
void Vector2D::multiplyByMatrix (Vector2D &arg) {
    assert (arg.size == this->size);

    std::vector <double> tdata (size * size);
    double tmp = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            tmp = 0;
            for (int k = 0; k < size; ++k) {
                tmp += this->elementAt(i, k) * arg.elementAt(k, j);
            }

            tdata[size * i + j] = tmp;
        }
    }
    this->data = tdata;
}


void Vector2D::multByVectorModifyVector (Vector1D &arg) {
    assert (arg.size == this->size);

    std::vector <double> tdata (arg.data.size());
    for (int i = 0; i < size; ++i) {
        tdata[i] = mulVectors (&(this->data[0]) + size * i, &(arg.data[0]), size);
    }
    arg.data = tdata;
}


void Vector2D::subtractMatrix (Vector2D &arg) {
    assert (arg.size == this->size);

    for (int i = 0; i < data.size(); ++i) {
        data[i] -= arg.data[i];
    }
}


double Vector2D::writeTo(int y, int x, double arg) {
    assert (x < size && y < size);

    int index = size * y + x;
    return data[index] = arg;
}
