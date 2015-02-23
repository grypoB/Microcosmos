#ifndef PARTICULE_H
#define PARTICULE_H

#include "geometry.h"

typedef struct Particule PARTICULE;

// constructer
int part_null(); // create particule with default settings
int part_create(double radius, POINT center, VECTOR speed);
bool part_deletePart(int partID);

// getters, do not modify the particle (return a default value if ID not found)
POINT part_getCenter(int partID);
VECTOR part_getSpeed(int partID);
VECTOR part_getForce(int partID);
VECTOR part_getAcceleration(int partID);
double part_getRadius(int partID);
double part_getMass(int partID);

// setters, return true if set was successful
bool part_setCenter(int partID, POINT center);
bool part_setSpeed(int partID, VECTOR speed);
bool part_setForce(int partID, VECTOR force);
//void part_setAcceleration(PARTICULE *part, VECTOR acceleration);
bool part_setRadius(int partID, double radius);

// other functions
static void part_initMass(PARTICULE *part);
//void part_addSpeed(PARTICULE *part, VECTOR deltaSpeed);
//void part_addForce(PARTICULE *part, VECTOR deltaForce);
//bool part_updateAcc(PARTICULE *part); // use the force applied to the particule to calculate the corresponding acceleration, return if true successful (mass!=0)

// utility
static PARTICULE* part_lastPart();
static PARTICULE* part_emptySlot();
static PARTICULE* part_findPart(int partID);

#endif
