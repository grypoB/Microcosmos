/* Nom: geometry.c
 * Description: module qui gère les calculs géométriques
 * Date: 08.02.2014
 * version : 1.1
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "geometry.h"
#include "constantes.h"

// give the y coord of point C
// by linear interpolation of the point A and B
// return 0 if A and B are too clolse (EPSILON_ZERO)
double linear_interpolation(double xC,
                            double xA, double yA,
                            double xB, double yB) {
    if (abs(xA-xB) > EPSILON_ZERO) {
        return yA + (yB-yA)/(xB-xA) * (xC-xA);
    } else {
        return 0;
    }
}


POINT point_null()
{
	POINT p;
	p.x = 0;
	p.y = 0;
	return p;
}


VECTOR vector_null()
{
	VECTOR v;
	v.x = 0;
	v.y = 0;
	return v;
}


VECTOR vector_create(double x, double y)
{
	VECTOR v;
	v.x = x;
	v.y = y;
	return v;
}

POINT point_create(double x, double y)
{
	POINT p;
	p.x = x;
	p.y = y;
	return p;
}
                            
VECTOR vector_sum(VECTOR v1, VECTOR v2)
{
	VECTOR vf;
	vf.x= v1.x + v2.x;
	vf.y= v1.y + v2.y;
	return vf;
}

double vector_norm(VECTOR v)
{
	return sqrt(v.x * v.x + v.y  * v.y);
}

VECTOR vector_multiply(VECTOR v, double scalar)
{
	v.x = v.x * scalar;
	v.y = v.y * scalar;
	return v;
}

double point_distance(POINT p1, POINT p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

POINT point_translate(POINT p, VECTOR v)
{
	p.x = p.x + v.x;
	p.y = p.y + v.y;
	return p;
}
