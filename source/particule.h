#ifndef PARTICULE_H
#define PARTICULE_H

#include "geometry.h"

typedef struct Particule PARTICULE;

// constructer
int part_null(); // create particule with default settings
int part_create(double radius, POINT center, VECTOR speed);

// getters
POINT part_getCenter(int partID);
VECTOR part_getSpeed(int partID);
VECTOR part_getForce(int partID);
VECTOR part_getAcceleration(int partID);
double part_getRadius(int partID);
double part_getMass(int partID);

// setters
bool part_setCenter(int partID, POINT center);
bool part_setSpeed(int partID, VECTOR speed);
bool part_setForce(int partID, VECTOR force);
//void part_setAcceleration(PARTICULE *part, VECTOR acceleration);
bool part_setRadius(int partID, double radius);

// other functions
static void part_initMass(PARTICULE *part);
static PARTICULE* part_emptySlot();
//void part_addSpeed(PARTICULE *part, VECTOR deltaSpeed);
//void part_addForce(PARTICULE *part, VECTOR deltaForce);
//bool part_updateAcc(PARTICULE *part); // use the force applied to the particule to calculate the corresponding acceleration, return if true successful (mass!=0)

#endif
