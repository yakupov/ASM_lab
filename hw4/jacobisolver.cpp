#include "jacobisolver.h"

JacobiSolver::JacobiSolver(std::string filename) :
    x (filename),
    c (x.getSize()),
    g (x) {

    Vector2D a (filename);
    Vector1D b (filename);

    //x already done
    Vector2D d (a);
    d.becomeInverseDiag();

    d.multByVectorModifyVector(g); //g done

    d.multiplyByMatrix(a);
    c.subtractMatrix(d); //done

    eps = 0.00001;
}


void JacobiSolver::processJacobi() {
    Vector1D xnext (x);
    double subAbs = eps + 1;
    int i = 0;

    while (subAbs > eps) {
        c.multByVectorModifyVector(xnext);
        xnext.addVector(g);
        x.subtractVector(xnext);
        subAbs = x.abs();
        x = xnext;
    }

    std::cout << "x:\n";
    for (int i = 0; i < x.getSize(); ++i) {
        std::cout << x.elementAt(i) << "\n";
    }
}
