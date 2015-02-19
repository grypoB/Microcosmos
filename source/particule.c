#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>
#include "particule.h"
#include "geometrie.h"



struct Particule {
   double radius;
   double mass;

   POINT center;

   VECTOR speed;
   VECTOR force;
   VECTOR acceleration;
};


// constructers
PARTICULE part_null() {
    PARTICULE part;

    part.radius = 0;
    part_nullMass(&part);
    part.center = point_null();
    part.speed = vector_null();
    part.force = vector_null();
    part.acceleration = vector_null();

    return part;
}

PARTICULE part_create(double radius, POINT center, VECTOR speed) {
    PARTICULE part;

    part.radius = radius;
    part_nullMass(&part);
    part.center = center;
    part.speed = speed;
    part.force = vector_null();
    part.acceleration = vector_null();

    return part;
}


// Getters for the varius field of the structure Particule

POINT part_getCenter(PARTICULE part) {
    return part.center;
}

VECTOR part_getSpeed(PARTICULE part) {
    return part.speed;
}

VECTOR part_getForce(PARTICULE part) {
    return part.force;
}

VECTOR part_getAcceleration(PARTICULE part) {
    return part.acceleration;
}
double part_getRadius(PARTICULE part) {
    return part.radius;
}

double part_getMass(PARTICULE part) {
    return part.center;
}


// Setters for the various field of the structure Particule
// mass is not settable without : part_initMass

void part_setCenter(PARTICULE *part, POINT center) {
    part->center = center;
}

void part_setSpeed(PARTICULE *part, VECTOR speed) {
    part->speed = speed;
}

void part_setForce(PARTICULE *part, VECTOR force) {
    part->force = force;
}

void part_setAcceleration(PARTICULE *part, VECTOR acceleration) {
    return part->acceleration = acceleration;
}

// return if successful or not (radius must be >=0)
bool part_setRadius(PARTICULE *part, double radius) {
    if (radius>=0) {
        part->radius = radius;
        part_initMass(part)
        return true;
    }
    else {
        return false;
    }
}


// Various function
static void part_initMass(PARTICULE *part) {
    part->mass = pow(part->radius, 2) * KMASS;
}
