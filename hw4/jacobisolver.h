#ifndef JACOBISOLVER_H
#define JACOBISOLVER_H

#include <iostream>
#include <string>
#include "Vector1D.h"
#include "Vector2D.h"

class JacobiSolver {
public:
    JacobiSolver(std::string filename);
    void processJacobi();
    double setEps (double arg) {
        if (arg > 0.0000000000000000000000001)
            return this->eps = arg;
        return this->eps;
    }

private:
    Vector2D a;
    Vector1D b;
    Vector1D x;
    Vector2D c;
    Vector1D g;

    double eps;
};

#endif // JACOBISOLVER_H
