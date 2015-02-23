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

#if 1
// function to do (2015-02-19)
POINT point_null(); // return a point of coord (0,0)
VECTOR vector_null(); // return the null vector (0,0)

double vector_norm(VECTOR v);

VECTOR vector_sum(VECTOR v1, VECTOR v2);
VECTOR vector_multiply(VECTOR v, double scalar);
#endif

#endif
