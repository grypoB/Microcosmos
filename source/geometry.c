/* Nom: geometry.c
 * Description: module qui gère les calculs géométriques
 * Date: 15.03.2015
 * version : 1.1
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "geometry.h"
#include "constantes.h"

// Point functions
POINT point_null()
{
	return (POINT) {0, 0};
}

POINT point_create(double x, double y)
{
	return (POINT) {x, y};
}

double point_distance(POINT p1, POINT p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

POINT point_translate(POINT p, VECTOR v)
{
	return (POINT) {p.x+v.x, p.y+v.y};
}


// vector fcts
VECTOR vector_null()
{
	return (VECTOR) {0, 0};
}

VECTOR vector_create(double x, double y)
{
	return (VECTOR) {x, y};
}
                            
VECTOR vector_sum(VECTOR v1, VECTOR v2)
{
	return (VECTOR) {v1.x+v2.x, v1.y+v2.y};
}

VECTOR vector_multiply(VECTOR v, double scalar)
{
	return (VECTOR) {v.x*scalar, v.y*scalar};
}

double vector_norm(VECTOR v)
{
	return sqrt(v.x * v.x + v.y  * v.y);
}

/** Return vector of norm vec_norm and of direction p1->p2
 *  Return the null vector if p1==p2
 *  Parameters :
 *      - point p1 and p2 : defining the line on which the returned
 *                          vector must fit
 *      - vec_norm : norm of the vector to return 
 */
VECTOR vector_fitLine(POINT p1, POINT p2, double vec_norm) {
    double dist = point_distance(p1, p2);

    if (dist == 0.) {
        return vector_null();
    }

    return (VECTOR) {vec_norm/dist * (p2.x-p1.x),
                     vec_norm/dist * (p2.y-p1.y)};
}

// ===================================================================
// Other functions

// give the y coord of point C
// by linear interpolation of the point A and B
// return 0 if A and B are too close (EPSILON_ZERO)
double linear_interpolation(double xC,
                            double xA, double yA,
                            double xB, double yB) {
    if (fabs(xA-xB) > EPSILON_ZERO) {
        return yA + (yB-yA)/(xB-xA) * (xC-xA);
    } else {
        return 0;
    }
}


// Return a random number in [0,1).
double randomDouble() {
    static bool initialized = false;

    if (!initialized) {
        #ifdef DEBUG
        printf("Initialized random\n");
        #endif
        srand(time(NULL)); // initialized list of random number
        initialized = true;
    }

    // rand return int in [1, RAND_MAX-1]
    return (double) (rand()-1)/(RAND_MAX-1);
}
