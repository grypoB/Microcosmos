#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "constante.h"
#include "particule.h"
#include "error.h"

#define PART_TAB_SIZE MAX_RENDU1
#define TAB_GROWTH_RATIO 2

typedef struct Particule PARTICULE;

static void part_updateForce();
static void part_updateAcc();
static void part_updateSpeed(double delta_t);
static void part_updatePos(double delta_t);

static void part_initMass(PARTICULE *part);

static PARTICULE* part_lastPart();
static PARTICULE* part_nextEmptySlot();
static PARTICULE* part_findPart(int partID);

struct Particule {

    bool locked; // a locked particle cannot move
    int id;

    double radius;
    double mass;

    POINT center;

    VECTOR speed;
    VECTOR force;
    VECTOR acceleration;
};

static PARTICULE *partTab = NULL;
static int partNB = 0;

void donnees_particules(const char * tab) 
{
	PARTICULE particule;
	particule.nb_particules = 0; 
	int i = 0;
	char fin;
	
	if(!(sscanf(tab, "%lf", &particule.nb_particules)))
	{
		//error_lect_nb_elements(particule);
	}
	
	for(i=0; i<particule.nb_particules; i++)
	{
		sscanf(tab, "%lf" "%lf" "%lf" "%lf" "%lf", &particule.rayon, &particule.posx, &particule.posy, &particule.vx, &particule.vy);
	}	
	
	sscanf(tab, "%c", &fin);
	char fin_entitee[] = "FIN_LISTE";
	if(!(strcmp(fin_entitee, &fin))) 
	{	
		//error_lecture_elements();
	}
}



// -----------
// constructer
// return the id of the particle (>=0), if there was an error, return UNASSIGNED (= -1)
int part_create(double radius, POINT center, VECTOR speed) {
    static int id = 0;
    int returnID = 0;
    PARTICULE *pPart = NULL;

    if (vector_norm(speed) > MAX_VITESSE) {
        error_vitesse_partic(ERR_PARTIC, id);
        returnID = UNASSIGNED; 
    } else if (radius>=RMAX || radius<=RMIN) {
        error_rayon_partic(ERR_PARTIC, id);
        returnID = UNASSIGNED;
    } else { 
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
    }

    id++;

    return returnID;
}

// -----------
// destructors
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
        free(partTab);
    }
}

// -----------
// Getters for the varius field of the structure Particule
POINT part_getCenter(int partID) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL) {
        return pPart->center;
    } else {
        return point_null();
    }
}

VECTOR part_getSpeed(int partID) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL) {
        return pPart->speed;
    } else {
        return vector_null();
    }
}

VECTOR part_getForce(int partID) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL) {
        return pPart->force;
    } else {
        return vector_null();
    }
}

VECTOR part_getAcceleration(int partID) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL) {
        return pPart->acceleration;
    } else {
        return vector_null();
    }
}

double part_getRadius(int partID) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL) {
        return pPart->radius;
    } else {
        return 0;
    }
}

double part_getMass(int partID) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL) {
        return pPart->mass;
    } else {
        return 0;
    }
}


// -----------
// Setters for the various field of the structure Particule
// return true if the change was made, false otherwise
// mass is not settable without : part_initMass
bool part_setCenter(int partID, POINT center) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL) {
        pPart->center = center;
        return true;
    } else {
        return false;
    }
}

bool part_setSpeed(int partID, VECTOR speed) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL) {
        pPart->speed = speed;
        return true;
    } else {
        return false;
    }
}

bool part_setRadius(int partID, double radius) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL && radius<=RMAX && radius>=RMIN) {
        pPart->radius = radius;
        part_initMass(pPart);
        return true;
    } else {
        return false;
    }
}

bool part_setLock(int partID, bool lock) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL) {
        pPart->locked = lock;
        return true;
    } else {
        return false;
    }
}


// -----------
// Various functions
static void part_initMass(PARTICULE *part) {
    part->mass = pow(part->radius, 2) * KMASSE;
}


// ----------
// force/simulation related
// all these fct act on the entire table partTab of particles
bool part_nextTick(double delta_t) {
    if (partTab!=NULL) {
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

//static part_collisionBlackHole() {}

static void part_updateForce() {
    int i=0, j=0;

    double distance = 0;
    double seuil_d = 0;
    double force_norm = 0;

    for (i=0 ; i<partNB ; i++) {
        partTab[i].force = vector_null();

        // TODO add black holes effet


        // TODO
    }

    for (i=0 ; i<partNB-1 ; i++) {
        for (j=i+1 ; j<partNB ; j++) {
            distance = point_distance(partTab[i].center, partTab[j].center);
            seuil_d = partTab[i].radius + partTab[j].radius + fmin(partTab[i].radius, partTab[j].radius);

            if (distance < 3*seuil_d) {
                if (distance < EPSILON_ZERO) {
                    force_norm = MAX_REP;
                } else if (distance < seuil_d) {
                    force_norm = linear_interpolation(distance, 0, MAX_REP, seuil_d, 0);
                } else if (distance < 2*seuil_d) {
                    force_norm = linear_interpolation(distance, seuil_d, 0, 2*seuil_d, MAX_ATTR);
                } else { // distance < 3*seuil_d
                    force_norm = linear_interpolation(distance, 2*seuil_d, MAX_ATTR, 3*seuil_d, 0);
                }

                // application of the force
                if (distance < EPSILON_ZERO) {
                    partTab[i].force = vector_create(0, force_norm);
                }
                else { // general case
                    partTab[i].force = vector_create(force_norm/distance
                                                     * (partTab[j].center.x-partTab[i].center.x),
                                                     force_norm/distance
                                                     * (partTab[j].center.y-partTab[i].center.y));

                }
                partTab[j].force = vector_multiply(partTab[i].force, -1);

            }
        }
    }
}

static void part_updateAcc() {
    int i=0;
    for (i=0 ; i<partNB ; i++) {
        partTab[i].acceleration = vector_multiply(partTab[i].force, 1/partTab[i].mass);
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
                partTab[i].speed = vector_multiply(partTab[i].speed, (MAX_VITESSE/speed_norm));
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

// ----------
// other sims functions
int part_totalNB() {
    return partNB;
}

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

// -----------
// utilities function (finding and managing the particules in the data structure)
static PARTICULE* part_nextEmptySlot() {
    static int partTabSize = 0;
    int i = 0;

    if (partTab==NULL) { // if table doesn't exist
        partTab = malloc(sizeof(PARTICULE)*PART_TAB_SIZE);
        if (partTab==NULL) error_msg("MEM allocation failed");
        partTabSize = PART_TAB_SIZE;
    } else if (partNB == partTabSize) { // if table full, increase its size
        PARTICULE *newPartTab = malloc(sizeof(PARTICULE)*partTabSize*TAB_GROWTH_RATIO);
        if (newPartTab==NULL) error_msg("MEM allocation failed");
        for (i=0; i<partNB; i++) { 
            newPartTab[i] = partTab[i];
        }
        free(partTab);
        partTab = newPartTab;
        partTabSize*=2;
    }

    return &partTab[partNB];
}

static PARTICULE* part_lastPart() {
    if (partTab!=NULL && partNB>0) { 
        return &partTab[partNB-1];
    } else {
        return NULL;
    }
}

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
