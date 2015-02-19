#ifndef PARTICULE_H
#define PARTICULE_H

typedef struct Particule PARTICULE;

// constructer
PARTICULE part_null(); // create particule with default settings
PARTICULE part_create(double radius, POINT center, VECTOR speed);

// getters
POINT part_getCenter(PARTICULE particule);
VECTOR part_getSpeed(PARTICULE part);
VECTOR part_getForce(PARTICULE part);
VECTOR part_getAcceleration(PARTICULE part);
double part_getRadius(PARTICULE part);
double part_getMass(PARTICULE part);

// setters
void part_setCenter(PARTICULE *part, POINT center);
void part_setSpeed(PARTICULE *part, VECTOR speed);
void part_setForce(PARTICULE *part, VECTOR force);
void part_setAcceleration(PARTICULE *part, VECTOR acceleration);
bool part_setRadius(PARTICULE *part, double radius); // return if successful (radius>=0)



static void part_initMass(PARTICULE *part);

#endif
