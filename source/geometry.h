/* Nom: geometry.h
 * Description: module qui gère les calculs géométriques
 * Date: 08.02.2014
 * version : 1.1
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef GEOMETRIE_H
#define GEOMETRIE_H

typedef struct {
    double x;
    double y;
} POINT;

typedef struct {
    double x;
    double y;
} VECTOR;


// return a point of coord (0, 0)
POINT point_null();
// return point of coord (x, y)
POINT point_create(double x, double y); 
// return the translation of point p by vector v
POINT point_translate(POINT p, VECTOR v);
// return the (euclidiian) distance between to points
double point_distance(POINT p1, POINT p2);


// return the null vector (0,0)
VECTOR vector_null();
// return vector of coord (x ,y)
VECTOR vector_create(double x, double y);
// return the sum of two vectors (v1, v2).
VECTOR vector_sum(VECTOR v1, VECTOR v2);
// return the vector v multiplied by a scalar
VECTOR vector_multiply(VECTOR v, double scalar);
// return the norm of v
double vector_norm(VECTOR v);


// give the y coord of point C
// by linear interpolation of the point A and B
// return 0 if A and B are too close (EPSILON_ZERO)
double linear_interpolation(double xC,
                            double xA, double yA,
                            double xB, double yB);

#endif
