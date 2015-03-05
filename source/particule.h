#ifndef PARTICULE_H
#define PARTICULE_H

#include "geometry.h"

// all particules are stored with a static in .c
// creating one returns an unique ID which is used to reference to this particle later

// file lecture
bool read_dataPart(const char *string);

// constructer
int  part_create(double radius, POINT center, VECTOR speed);
bool part_deletePart(int partID);
void part_deleteAll();

// getters, do not modify the particle (return a default value if ID not found)
double part_getRadius(int partID);
double part_getMass  (int partID);
POINT  part_getCenter(int partID);
VECTOR part_getSpeed (int partID);
VECTOR part_getForce (int partID);
VECTOR part_getAcceleration(int partID);

// setters, return true if set was successful (i.e. particle was found)
bool part_setCenter(int partID, POINT  center);
bool part_setSpeed (int partID, VECTOR speed);
bool part_setRadius(int partID, double radius);
bool part_setLock  (int partID, bool lock);

// force/simultaion related
int part_totalNB();
bool part_nextTick(double delta_t);
int part_closestPart(POINT point); // return ID of closest particle

#endif
