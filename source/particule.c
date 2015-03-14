/* Nom: particule.c
 * Description: module qui gère les particules
 * Date: 08.02.2014
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "constantes.h"
#include "particule.h"

// default size of the tab to store the particule in
#define PART_TAB_SIZE MAX_RENDU1

typedef struct Particule {

    bool locked; // a locked particle cannot move
    int id; // the unique identifier of a particule

    double radius; // must be included in [RMIN, RMAX]
    double mass; // depends of the radius (see init_mass)

    POINT center; // position of the center of the particule

    VECTOR speed; // its norm must be below 10
    VECTOR force;
    VECTOR acceleration;

} PARTICULE;

static void part_initMass(PARTICULE *part);

static double part_calcForce (PARTICULE *p1, PARTICULE *p2, double distance);


// tab to store all the particle in
static PARTICULE partTab[PART_TAB_SIZE];
// strore the current number of particles
static int partNB = 0;


// from an input string, creates a particule
// Expected format (all in double): radius posx posy vx vy 
// prints errors if it couldn't read string
bool part_readData(const char *string) {
    double param[5] = {0};
    bool success = false;

    if (sscanf(string, "%lf %lf %lf %lf %lf", &param[0], &param[1], &param[2],
                                              &param[3], &param[4])==5) {

        if (part_create(param[0], point_create(param[1], param[2]),
            vector_create(param[3], param[4])) != UNASSIGNED) {
            success = true;
        }

    } else {
        error_lecture_elements(ERR_PARTIC, ERR_PAS_ASSEZ);
    }

    return success;
}

// check if given params are valid (see part_create)
// if verbose and if param aren't valid
// it will call the appropriate error fct sending it the  given origin and
// id number (see error_vitesse_partic and error_rayon_partic)
bool part_validParams(double radius, POINT center, VECTOR speed,
                      bool verbose, ERREUR_ORIG origin, int id) {
    bool valid = false;

    if (vector_norm(speed) > MAX_VITESSE) {
        if (verbose) {
            error_vitesse_partic(origin, id);
        }
    } else if (radius>=RMAX || radius<=RMIN) {
        if (verbose) {
            error_rayon_partic(origin, id);
        }
    } else {
        valid = true;
    }

    return valid;
}


// -----------
// constructor
// return the id of the particule (>=0)
// return UNASSIGNED if radius isn't in [RMIN, RMAX], 
// or if speed's norm > MAX_VITESSE
int part_create(double radius, POINT center, VECTOR speed) {
    static int id = 0; // static counter (for unique identifier)
    int returnID = UNASSIGNED;
    PARTICULE *pPart = NULL;

    if (part_validParams(radius, center, speed, true, ERR_PARTIC, id)) {
        if (partNB < PART_TAB_SIZE) {
            pPart = &partTab[partNB];
            pPart->locked = false;
            pPart->id = id;
            pPart->radius = radius;
            part_initMass(pPart);
            pPart->center = center;
            pPart->speed = speed;
            pPart->force = vector_null();
            pPart->acceleration = vector_null();

            returnID = id;
            partNB++;

            #ifdef DEBUG
            printf("Part id no %d created\n", id);
            #endif
        } else {
            error_msg("Not ebough memory to create particule");
        }
    }

    id++;

    return returnID;
}


// ----------
// Simulation related functions
// calc force between the 2 first particules and prints it
void particule_force_rendu1() {
	double force_norm = 0;
    double distance = 0;

    if (partNB>=2) {
        distance = point_distance(partTab[0].center, partTab[1].center);
        force_norm = part_calcForce(&partTab[0], &partTab[1], distance);
        printf("%8.3f\n", force_norm);
    } else {
        error_msg("Not enough particles for Force mode (need at least 2)");
    }
}


static double part_calcForce(PARTICULE *p1, PARTICULE *p2, double distance) {
    double force_norm = 0;
    double seuil_d = p1->radius + p2->radius + fmin(p1->radius, p2->radius);

    if (distance < 3*seuil_d) {
        if (distance < EPSILON_ZERO) {
            force_norm = MAX_REP;
        } else if (distance < seuil_d) {
            force_norm = linear_interpolation(distance, 0, MAX_REP,
                                                        seuil_d, 0);
        } else if (distance < 2*seuil_d) {
            force_norm = linear_interpolation(distance, seuil_d, 0,
                                                        2*seuil_d, MAX_ATTR);
        } else { // distance < 3*seuil_d
            force_norm = linear_interpolation(distance, 2*seuil_d, MAX_ATTR,
                                                        3*seuil_d, 0);
        }
    }

    return force_norm;
}


// -----------
// set the mass of a particule (proportional to radius^2)
static void part_initMass(PARTICULE *part) {
    part->mass = pow(part->radius, 2) * KMASSE;
}
