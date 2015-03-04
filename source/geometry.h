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

<<<<<<< HEAD
// // //

typedef struct  // PARTICULE
{
	double nb_particules;
	double rayon;
	double posx;
	double posy;
	double vx;
	double vy;
}PARTICULE;

typedef struct //GENERATEUR
{
	double nb_de_generateurs;
	double rgen;
	double posx;
	double posy;
	double vpi_x;
	double vpi_y;
}GENERATEUR;

typedef struct //TROU NOIR
{
	double nb_de_trous_noirs;
	double posx;
	double posy;
}TROU_NOIR;

#if 1
// function to do (2015-02-19)
=======
>>>>>>> 9300a8756d018df58fffbb8d37b646df49198ec9
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
