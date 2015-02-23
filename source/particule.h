#ifndef PARTICULE_H
#define PARTICULE_H

#include "geometry.h"

typedef struct Particule PARTICULE;

// constructer
int  part_create(double radius, POINT center, VECTOR speed);
bool part_deletePart(int partID);

// getters, do not modify the particle (return a default value if ID not found)
double part_getRadius(int partID);
double part_getMass  (int partID);
POINT  part_getCenter(int partID);
VECTOR part_getSpeed (int partID);
VECTOR part_getForce (int partID);
VECTOR part_getAcceleration(int partID);

// setters, return true if set was successful
bool part_setCenter(int partID, POINT  center);
bool part_setSpeed (int partID, VECTOR speed);
bool part_setRadius(int partID, double radius);
bool part_setLock  (int partID, bool lock);

// force/simultaion related
bool part_nextTick(double delta_t);
static void part_updateForce();
static void part_updateSpeed(double delta_t);
static void part_updatePos(double delta_t);

// other functions
static void part_initMass(PARTICULE *part);
//bool part_updateAcc(PARTICULE *part); // use the force applied to the particule to calculate the corresponding acceleration, return if true successful (mass!=0)

// utility
static PARTICULE* part_lastPart();
static PARTICULE* part_nextEmptySlot();
static PARTICULE* part_findPart(int partID);

#endif
