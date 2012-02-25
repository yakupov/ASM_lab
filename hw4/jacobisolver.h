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
        if (arg > 0)
            return this->eps = arg;
        return this->eps;
    }

private:
    Vector1D x;
    Vector2D c;
    Vector1D g;

    double eps;
};

#endif // JACOBISOLVER_H
