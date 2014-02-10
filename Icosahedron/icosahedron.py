from numpy import *
import numpy as numpy
import math

def norm(V):
    return math.sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);

radius = 1;

phi = (1+math.sqrt(5))/2;

vertices = array([ [   0,    1,  phi], 
                 [   0,   -1,  phi],
                 [   0,    1, -phi], 
                 [   0,   -1, -phi], 
                 [   1,  phi,    0], 
                 [  -1,  phi,    0],  
                 [   1, -phi,    0], 
                 [  -1, -phi,    0],  
                 [ phi,    0,    1], 
                 [-phi,    0,    1], 
                 [ phi,    0,  - 1],
                 [-phi,    0,   -1]]);


vertices *= (radius/math.sqrt(1+phi*phi));

Z = vertices[0][:].copy();
X = vertices[1][:].copy();
Y = numpy.cross(Z,X);
X = numpy.cross(Y,Z);
X /= norm(X);
Y /= norm(Y);
Z /= norm(Z);

transform = column_stack((X,Y,Z));

#   Z = xyz(1,:)';       %Z passes through vertex 1
#   X = xyz(2,:)';   % Choose adjacent vertex as an approximate X
#   Y = cross(Z,X);  % Y is perpendicular to Z and this approx X
#   X = cross(Y,Z);  % Final X is perpendicular to Y and Z
#   X = X/norm(X); Y = Y/norm(Y); Z = Z/norm(Z);  % Ensure unit vectors
#   xyz = ([X Y Z]'*xyz')';  % Transform points;


vertices = dot(vertices,transform);

for row in range(size(vertices,0)):
    for col in range(size(vertices,1)):
        vertices[row][col] = round(vertices[row][col], 4);

print(vertices);

