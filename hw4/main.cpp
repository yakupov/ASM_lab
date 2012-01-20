#include <iostream>
#include <vector>
#include "jacobisolver.h"

int main () {
    JacobiSolver aa("input.txt");
    aa.processJacobi();

    return 0;
}
