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

POINT point_null(); // return a point of coord (0,0)
POINT point_create(double x, double y);
double point_distance(POINT p1, POINT p2);
POINT point_translate(POINT p, VECTOR v);

VECTOR vector_null(); // return the null vector (0,0)
VECTOR vector_create(double x, double y);
VECTOR vector_sum(VECTOR v1, VECTOR v2);
VECTOR vector_multiply(VECTOR v, double scalar);
double vector_norm(VECTOR v);


// give the y coord of point C
// by linear interpolation of the point A and B
// return 0 if A and B are too clolse (EPSILON_ZERO)
double linear_interpolation(double xC,
                            double xA, double yA,
                            double xB, double yB);

#endif
