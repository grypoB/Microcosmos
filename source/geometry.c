#include <stdlib.h>
#include <stdio.h>


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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct   //COORDONNEE D'UN POINT
{
	double x;
	double y;
}COORDONNEE;

typedef struct   //COORDONNEE D'UN VECTEUR
{
	double x;
	double y;
}VECTEUR;

typedef struct  // PARTICULE
{
	double rayon;
	COORDONNEE centre;
}PARTICULE;

int info_particule(PARTICULE particule_1)
{
	//printf("Entrer le rayon de la particule:\n");
	scanf("%lf", &(particule_1.rayon)); 
	
	//printf("Entrer la coordonnée x du centre de la particule:\n");
	scanf("%lf", &(particule_1.centre.x)); 
	
	//printf("Entrer la coordonnée y du centre de la particule:\n");
	scanf("%lf", &(particule_1.centre.y)); 
	
	return 0;
}

double distances_particules(PARTICULE particule_1, PARTICULE particule_2)
{
	double distance_abscisse = particule_1.centre.x - particule_2.centre.x;
	double distance_ordonnee = particule_1.centre.y - particule_2.centre.y;
	double distance_entre_particules = sqrt(distance_abscisse * distance_abscisse + distance_ordonnee  * distance_ordonnee);
	
	return distance_entre_particules;
}

VECTEUR addition_vecteurs(VECTEUR vecteur_1, VECTEUR vecteur_2)
{
	VECTEUR vecteur_resultant;
	vecteur_resultant.x= vecteur_1.x + vecteur_2.x;
	vecteur_resultant.y= vecteur_1.y + vecteur_2.y;
	return vecteur_resultant;
}

double norme_vecteur(VECTEUR vecteur_1)
{
	double norme_du_vecteur;
	norme_du_vecteur =sqrt(vecteur_1.x * vecteur_1.x + vecteur_1.y  * vecteur_1.y);
	return norme_du_vecteur;
}

VECTEUR multiplication_vecteurs(VECTEUR vecteur, double a)
{
	vecteur.x = vecteur.x * a;
	vecteur.y = vecteur.y * a;
	return vecteur;
}


