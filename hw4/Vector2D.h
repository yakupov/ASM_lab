#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <vector>
#include <fstream>
#include <string>
#include <assert.h>
#include "Vector1D.h"
#include "mulvectors.h"

class Vector2D {
public:
    Vector2D (size_t size);
    Vector2D (const std::string &filename);
    Vector2D (const Vector2D& arg);
    void becomeInverseDiag ();                      //convert A to D^-1 (for Jacobi)
    double elementAt (int, int) const;
    int getSize () const {return size;}
    void multiplyByMatrix (Vector2D &arg);          //!!! writes result to this
    void multByVectorModifyVector (Vector1D &arg);  //!!! writes result to argument
    void subtractMatrix (Vector2D &arg);            //!!! writes result to this

protected:
    std::vector<double> data;
    int size;

private:
    double writeTo(int x, int y, double arg);
};

#endif // VECTOR2D_H
