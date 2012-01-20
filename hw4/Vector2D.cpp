#include "Vector2D.h"


Vector2D::Vector2D(size_t dimension) {
    this->dimension = dimension;

    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            if (i != j) {
                data.push_back (0);
            } else {
                data.push_back (1);
            }
        }
    }
}


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



void Vector2D::becomeInverseDiag() {
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            if (i != j) {
                this->writeTo(i, j, 0);
            } else {
                writeTo(i, i, 1/elementAt(i, i));
            }
        }
    }
}



double Vector2D::elementAt(int y, int x) {
    if (x >= dimension || y >= dimension) {
        std::cout << "index OOB 2d\n" << dimension << '\n' << x << y << "  get \n";
        throw ("index out of bounds");
    }

    int index = dimension * y + x;
    return data[index];
}


//in order to use mulVectors, we'd have to create %dimension new vectors
//that's too slow, so no SSE here
void Vector2D::multiplyByMatrix (Vector2D &arg) {
    if (arg.dimension != this->dimension) {
        std::cout << "size mismatch 2d mulmat\n";
        throw ("size mismatch");
    }

    std::vector <double> tdata (dimension * dimension);
    double tmp = 0;
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            tmp = 0;
            for (int k = 0; k < dimension; ++k) {
                tmp += this->elementAt(i, k) * arg.elementAt(k, j);
            }

            tdata[dimension * i + j] = tmp;
        }
    }
    this->data = tdata;
}


/*
Vector1D Vector2D::multiplyByVector (Vector1D &arg) {
    Vector1D res(arg);
    for (int i = 0; i < dimension; ++i) {
	res.data[i] = mulVectors (&(this->data[0]) + dimension * i, &(arg.data[0]), dimension);
    }
    return res;
}
*/

void Vector2D::multByVectorModifyVector (Vector1D &arg) {
    if (arg.dimension != this->dimension) {
        std::cout << "size mismatch 2d mulvec\n";
        throw ("size mismatch");
    }

    std::vector <double> tdata (arg.data.size());
    for (int i = 0; i < dimension; ++i) {
        tdata[i] = mulVectors (&(this->data[0]) + dimension * i, &(arg.data[0]), dimension);
    }
    arg.data = tdata;
}


void Vector2D::subtractMatrix (Vector2D &arg) {
    if (arg.dimension != this->dimension) {
        std::cout << "size mismatch 2d\n";
        throw ("size mismatch");
    }

    for (int i = 0; i < data.size(); ++i) {
        data[i] -= arg.data[i];
    }
}


double Vector2D::writeTo(int y, int x, double arg) {
    if (x >= dimension || y >= dimension) {
        std::cout << "index OOB 2d\n" << dimension << '\n' << y << x << "wrto\n";
        throw ("index out of bounds");
    }

    int index = dimension * y + x;
    return data[index] = arg;
}
