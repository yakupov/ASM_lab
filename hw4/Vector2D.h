#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <vector>
#include <fstream>
#include <string>
#include "Vector.h"
#include "Vector1D.h"
#include "mulvectors.h"

class Vector2D : public Vector {
public:
    Vector2D (size_t dimension);
    Vector2D (std::string filename);
    Vector2D (const Vector2D& arg);
    void becomeInverseDiag ();                      //convert A to D^-1 (for Jacobi)
    double elementAt (int, int);
    int getDimension () {return dimension;}
    void multiplyByMatrix (Vector2D &arg);          //!!! writes result to this
    //Vector1D multiplyByVector (Vector1D &arg);
    void multByVectorModifyVector (Vector1D &arg);  //!!! writes result to argument
    void subtractMatrix (Vector2D &arg);            //!!! writes result to this

private:
    double writeTo(int x, int y, double arg);
};

#endif // VECTOR2D_H
