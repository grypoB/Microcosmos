#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "constante.h"
#include "particule.h"
#include "geometry.h"

#define PART_TAB_SIZE MAX_RENDU1
#define DEFAULT_RADIUS 1

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


// -----------
// constructers

int part_null() {
    return part_create(DEFAULT_RADIUS, point_null(), vector_null());
}

int part_create(double radius, POINT center, VECTOR speed) {
    static int id = 0;
    id++;

    PARTICULE *pPart = part_emptySlot();

    pPart->locked = false;
    pPart->id = id; 
    pPart->radius = radius;
    part_initMass(pPart);
    pPart->center = center;
    pPart->speed = speed;
    pPart->force = vector_null();
    pPart->acceleration = vector_null();

    return pPart->id;
}

// -----------
// destructor
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

bool part_setForce(int partID, VECTOR force) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL) {
        pPart->force = force;
        return true;
    } else {
        return false;
    }
}

//void part_setAcceleration(PARTICULE *part, VECTOR acceleration) {

bool part_setRadius(int partID, double radius) {
    PARTICULE *pPart = part_findPart(partID);
    if (pPart != NULL) {
        pPart->radius = radius;
        part_initMass(pPart);
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

void part_addSpeed(PARTICULE *part, VECTOR deltaSpeed) {
    part->speed = vector_sum(part->speed, deltaSpeed);
}

void part_addForce(PARTICULE *part, VECTOR deltaForce) {
    part->force = vector_sum(part->force, deltaForce);
}

bool part_updateAcc(PARTICULE *part) {
    if (part->mass != 0) {
        part->acceleration = vector_multiply(part->force, 1/part->mass);
        return true;
    } else {
        return false;
    }
}


// -----------
// utilities function (management finding and managing the particules data structure)
static PARTICULE* part_emptySlot() {
    static int partTabSize = 0;
    int i = 0;


    // if not yet init
    if (partTab==NULL) {
        partTab = malloc(sizeof(PARTICULE)*PART_TAB_SIZE);
        if (partTab==NULL) {
            printf("MEM allocation failed");
            exit(EXIT_FAILURE);
        }
        partTabSize = PART_TAB_SIZE;
    }
    
    // if full
    if (partNB == partTabSize) {
        PARTICULE *newPartTab = malloc(sizeof(PARTICULE)*2);
        if (newPartTab==NULL) {
            printf("MEM allocation failed");
            exit(EXIT_FAILURE);
        }
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
    if (partTab != NULL && partNB>0) { 
        return &partTab[partNB-1];
    } else {
        return NULL;
    }
}

static PARTICULE* part_findPart(int partID) {
    int i=0;
    PARTICULE *pPart = NULL;
    
    if (partTab != NULL) {
        while (i < partNB && pPart != NULL) {
            if (partTab[i].id == partID) {
                pPart = &partTab[i];
            } 
        }
    }

    return pPart;
}
