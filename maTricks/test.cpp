#include <cstdio>
#include "matrix.hpp"
#include "vector.hpp"


double phi = (1+sqrt(5))/2;

double icosahedron[] = 
{
       0,    1,  phi, 
       0,   -1,  phi,
       0,    1, -phi, 
       0,   -1, -phi, 
       1,  phi,    0, 
      -1,  phi,    0,  
       1, -phi,    0, 
      -1, -phi,    0,  
     phi,    0,    1, 
    -phi,    0,    1, 
     phi,    0,  - 1,
    -phi,    0,   -1
};


int main() {

    double radius = 1;
    Matrix<double, 12, 3> verts(icosahedron);

    verts *= (double)(radius / sqrt(1+phi*phi));

    Vec3d Z = verts[0];
    Vec3d X = verts[1];
    Vec3d Y = cross(Z, X);
    X = cross(Y, Z);

    X = norm(X);
    Y = norm(Y);
    Z = norm(Z);

    Matrix<double, 3, 3> xform({X, Y, Z});

    verts = verts * xform;
    verts.print();

    float a[] = {2, 1, -1, 8,
                 -3, -1, 2, -11,
                  -2, 1, 2, -3};
    Matrix<float, 3, 4> arr(a);

    solve_GE(arr);

    arr.print();

}