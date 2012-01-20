#include "jacobisolver.h"

JacobiSolver::JacobiSolver(std::string filename) :
    a (filename),
    b (filename),
    x (b.getDimension()),
    c (a.getDimension()),
    g (b) {

    //x already done
    Vector2D d (a);
    d.becomeInverseDiag();

/*
    for (int i = 0; i < a.getDimension(); ++i) {
        for (int j = 0; j < a.getDimension(); ++j) {
            std::cout << a.elementAt(i, j) << "a\n";
        }
    }
    for (int i = 0; i < g.getDimension(); ++i) {
        std::cout << g.elementAt(i) << "g\n";
    }
*/

    d.multByVectorModifyVector(g); //g done

/*
    for (int i = 0; i < d.getDimension(); ++i) {
        for (int j = 0; j < d.getDimension(); ++j) {
            std::cout << d.elementAt(i, j) << "d\n";
        }
    }
    for (int i = 0; i < g.getDimension(); ++i) {
        std::cout << g.elementAt(i) << "g\n";
    }
*/

    d.multiplyByMatrix(a);

/*
    for (int i = 0; i < d.getDimension(); ++i) {
        for (int j = 0; j < d.getDimension(); ++j) {
            std::cout << d.elementAt(i, j) << "da\n";
        }
    }
*/

    c.subtractMatrix(d); //done

/*
    for (int i = 0; i < c.getDimension(); ++i) {
        for (int j = 0; j < c.getDimension(); ++j) {
            std::cout << c.elementAt(i, j) << "c\n";
        }
    }
*/
    eps = 0.00001;
}


void JacobiSolver::processJacobi() {
    Vector1D xnext (x);
    double subAbs = 1;
    int i = 0;

    while (subAbs > eps) {
        c.multByVectorModifyVector(xnext);
        xnext.addVector(g);
        x.subtractVector(xnext);
        subAbs = x.abs();
        x = xnext;

        //i++; if (i > 20) break;
    }

/*
    for (int i = 0; i < c.getDimension(); ++i) {
        for (int j = 0; j < c.getDimension(); ++j) {
            std::cout << c.elementAt(i, j) << "cc\n";
        }
    }


    for (int i = 0; i < g.getDimension(); ++i) {
        std::cout << g.elementAt(i) << "gg\n";
    }
*/
    std::cout << "x:\n";
    for (int i = 0; i < x.getDimension(); ++i) {
        std::cout << x.elementAt(i) << "\n";
        //std::cout << g.elementAt(i) << "\n";
        //std::cout << b.elementAt(i) << "\n";
        //std::cout << c.elementAt(i, i) << "\n";
    }
}
