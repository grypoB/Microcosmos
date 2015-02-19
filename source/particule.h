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
bool part_setRadius(PARTICULE *part, double radius); // return if true successful (radius>=0)

// other functions
static void part_initMass(PARTICULE *part);
void part_addSpeed(PARTICULE *part, VECTOR deltaSpeed);
void part_addForce(PARTICULE *part, VECTOR deltaForce);
bool part_updateAcc(PARTICULE *part); // use the force applied to the particule to calculate the corresponding acceleration, return if true successful (mass!=0)

#endif
