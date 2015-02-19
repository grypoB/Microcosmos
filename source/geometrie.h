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

VECTOR vector_sum(VECTOR v1, VECTOR v2);
#endif

#endif
