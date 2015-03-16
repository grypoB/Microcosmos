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
#include "graphic.h"
#include "particule.h"

// default size of the tab to store the particles
#define PART_TAB_SIZE MAX_RENDU1
// multiplier between each increase in size of partTab
#define TAB_GROWTH_RATIO 2

typedef struct Particule {

    bool locked; // a locked particle cannot move
    int id; // the unique identifier of a particle

    double radius; // must be included in [RMIN, RMAX]
    double mass; // depends of the radius (see init_mass)

    POINT center; // position of the center of the particle

    VECTOR speed; // its norm must be below 10
    VECTOR force;
    VECTOR acceleration;

} PARTICULE;

static void part_initMass(PARTICULE *part);

static void part_updateForce();
static void part_updateAcc();
static void part_updateSpeed(double delta_t);
static void part_updatePos(double delta_t);

static double part_calcForce (PARTICULE *p1, PARTICULE *p2, double distance);
static void   part_applyForce(PARTICULE *p1, PARTICULE *p2, double distance,
                                                           double force_norm);

// to navigate in the data structure
// static PARTICULE* part_firstPart();
static PARTICULE* part_lastPart();
static PARTICULE* part_nextEmptySlot();
static PARTICULE* part_findPart(int partID);


// tab to store all the particle in
// part_nextEmptySlot() inits it
static PARTICULE *partTab = NULL;
// store the current number of particles
static int partNB = 0;


// from an input string, creates a particle
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
// return the id of the particle (>=0)
// return UNASSIGNED if radius isn't in [RMIN, RMAX], 
// or if speed's norm > MAX_VITESSE
int part_create(double radius, POINT center, VECTOR speed) {
    static int id = 0; // static counter (for unique identifier)
    int returnID = UNASSIGNED;
    PARTICULE *pPart = NULL;

    if (part_validParams(radius, center, speed, true, ERR_PARTIC, id)) {
        pPart = part_nextEmptySlot();

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
    }

    id++;

    return returnID;
}

// -----------
// destructors (from particle ID)
// return false if the particle id doesn't exist (anymore)
bool part_deletePart(int partID) {
    PARTICULE *pPart     = part_findPart(partID);
    PARTICULE *pLastPart = part_lastPart();

    if (pPart != NULL && pLastPart != NULL) {
        *pPart = *pLastPart;
        partNB--;
        return true;
    } else {
        return false;
    }
}

void part_deleteAll() {

    if (partTab != NULL) {
        #if DEBUG
        int i;
        for (i=0 ; i<partNB ; i++) {
            printf("Freing particle no %d : %f %f %f %f %f\n", i, 
				   partTab[i].radius, partTab[i].center.x,partTab[i].center.y,
                   partTab[i].speed.x, partTab[i].speed.y);
        }
        #endif

        free(partTab);
        partNB = 0;
    }
}

// ----------
// a locked particle cannot move, but still exerces force on other particles
bool part_setLock(int partID, bool lock) {
    PARTICULE *pPart = part_findPart(partID);

    if (pPart != NULL) {
        pPart->locked = lock;
        return true;
    } else {
        return false;
    }
}


// ----------
// Simulation related functions
// calc force between the 2 first particles and prints it
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

// return the total number of current particles
int part_totalNB() {
    return partNB;
}

// return the id of the closest particle form a given point
// UNNASSIGNED if no particle exists
int part_closestPart(POINT point) {
    int i = 0;
    int partID = UNASSIGNED;
    double dist = 0;
    double newDist = 0;

    if (partTab!=NULL && partNB>0) {
        dist = point_distance(partTab[0].center, point);
        for (i=1 ; i<partNB ; i++) {
            newDist = point_distance(partTab[i].center, point);
            if (newDist < dist) {
                dist = newDist;
                partID = partTab[i].id;
            }
        }
    }

    return partID;
}


// ----------
// Simulation update fct
// update the status of all particles (speed + position): 
// calculating their attraction-repulsion force
// delta_t is the amount of time the "tick" lasts 
// return false if data structure of particle isn't set or if delta_t<0
bool part_nextTick(double delta_t) {

    if (partTab!=NULL && delta_t>=0.) {
        //part_collisionBlackHole();
        part_updateForce();
        part_updateAcc();
        part_updateSpeed(delta_t);
        part_updatePos(delta_t);
        return true;
    } else {
        return false;
    }
}

//static void part_collisionBlackHole() {}

static void part_updateForce() {
    int i=0, j=0;
    double distance = 0;
    double force_norm = 0;

    for (i=0 ; i<partNB ; i++) {
        partTab[i].force = vector_null();

        // TODO add black holes effects


        // TODO
    }

    for (i=0 ; i<partNB-1 ; i++) {
        for (j=i+1 ; j<partNB ; j++) {
            distance = point_distance(partTab[i].center, partTab[j].center);

            force_norm = part_calcForce(&partTab[i], &partTab[j], distance);

            part_applyForce(&partTab[i], &partTab[j], distance, force_norm);
        }
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

static void part_applyForce(PARTICULE *p1, PARTICULE *p2, double distance,
                                                          double force_norm) {

    if (distance < EPSILON_ZERO) {
        p1->force = vector_create(0, force_norm);
    } else { // general case
        p1->force = vector_create(force_norm/distance * 
								 (p2->center.x-p1->center.x), 
								 (force_norm/distance) * 
								 (p2->center.y-p1->center.y));
    }
    p2->force = vector_multiply(p1->force, -1);
}

static void part_updateAcc() {
    int i=0;

    for (i=0 ; i<partNB ; i++) {
        partTab[i].acceleration = vector_multiply(partTab[i].force, 
												  1/partTab[i].mass);
    }
}

static void part_updateSpeed(double delta_t) {
    int i=0;
    double speed_norm=0;

    for (i=0 ; i<partNB ; i++) {
        if (!partTab[i].locked) { // a locked particle doesn't build up speed
            partTab[i].speed = vector_sum(partTab[i].speed,
                               vector_multiply(partTab[i].speed, delta_t));
            speed_norm = vector_norm(partTab[i].speed);
            if (speed_norm > MAX_VITESSE) { // scale down the vector
                partTab[i].speed = vector_multiply(partTab[i].speed,
                                                   MAX_VITESSE/speed_norm);
            }
        }
    }
}

static void part_updatePos(double delta_t) {
    int i=0;

    for (i=0 ; i<partNB ; i++) {
        if (!partTab[i].locked) {
            partTab[i].center = point_translate(partTab[i].center,
                                vector_multiply(partTab[i].speed, delta_t));
        }
    }
}


// -----------
// set the mass of a particle (proportional to radius^2)
static void part_initMass(PARTICULE *part) {
    part->mass = pow(part->radius, 2) * KMASSE;
}

// -----------
// utilities functions (finding and managing the particles in the data
// structure)
// return pointer to an empty slot in the data structure,
// if not enough space, creates some
static PARTICULE* part_nextEmptySlot() {
    static int partTabSize = 0;
    int i = 0;

    if (partTab==NULL) { // if table doesn't exist

        partTab = malloc(sizeof(PARTICULE)*PART_TAB_SIZE);
        if (partTab==NULL) error_msg("MEM allocation failed");
        partTabSize = PART_TAB_SIZE;

    } else if (partNB == partTabSize) { // if table full, increases its size

        PARTICULE *newPartTab = NULL;
        newPartTab = malloc(sizeof(PARTICULE)*partTabSize*TAB_GROWTH_RATIO);
        if (newPartTab==NULL) error_msg("MEM allocation failed");

        for (i=0; i<partNB; i++) {  // copy old table into the new one
            newPartTab[i] = partTab[i];
        }

        free(partTab);
        partTab = newPartTab;
        partTabSize*=TAB_GROWTH_RATIO;
    }

    return &partTab[partNB];
}

/*
static PARTICULE* part_firstPart() {
    if (partTab!=NULL && partNB>0) {
        return &partTab[0];
    } else {
        return NULL;
    }
}
*/

// return NULL, if data structure not initialized
static PARTICULE* part_lastPart() {

    if (partTab!=NULL && partNB>0) {
        return &partTab[partNB-1];
    } else {
        return NULL;
    }
}

// return NULL if part not found
static PARTICULE* part_findPart(int partID) {
    int i=0;
    PARTICULE *pPart = NULL;
    
    if (partTab != NULL) {
        while (i<partNB && pPart!=NULL) {
            if (partTab[i].id == partID) {
                pPart = &partTab[i];
            }
        }
    }

    return pPart;
}

void part_draw(double radius, POINT center, VECTOR speed)
{	
	if(vector_norm(speed) == 0)
	{
		graphic_set_color3f(0, 0, 0);
	}
	else if(vector_norm(speed) == MAX_VITESSE) 
	{
		graphic_set_color3f(1, 0.2, 0.2);
	}
	else
	{
		//aucune idée comment faire pour interpoler pour l'instant
		graphic_set_color3f(1, 0.2, 0.2);    ?
	}
	
	//graphic_set_line_width(3.);
	graphic_draw_particule(center, radius, GRAPHIC_FILLED);
}
