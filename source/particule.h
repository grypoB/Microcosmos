/* Nom: particule.h
 * Description: module qui gère les particules
 * Date: 22.03.2015
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef PARTICULE_H
#define PARTICULE_H

#include "geometry.h"
#include "error.h"
#include <stdbool.h>

// all particles are stored with a static in .c
// creating one returns a unique ID (used to reference this particle later)
// don't forget to delete afterwards (to clear memory)

/*
Particles caracterised by : 
- an id
- a lock state (to be able to move or not)
- a radius
- a mass (dependent on radius)
- a center point
- a force, acceleration and speed vector
*/


// ------------
// constructor
// return UNNASIGNED if radius not in [RMIN, RMAX], or speed norm>MAX_VITESSE
// otherwise return the id (>=0) of the particle
int  part_create(double radius, POINT center, VECTOR speed);
// check if given params are valid (see part_create)
// if verbose and if param aren't valid it will call the appropriate error fct
// (sending it the given orgin and id number)
bool part_validParams(double radius, POINT center, VECTOR speed,
                      bool verbose, ERREUR_ORIG origin, int id);


// ------------
// destructors
// return false if particle wasn't found
bool part_deletePart(int partID);
// delete all particles, use with caution
void part_deleteAll();

// setter, return true if set was successful (i.e. partiule was found)
// locked particles can't move (see part_nextTick)
bool part_setLock  (int partID, bool lock);


// ------------
// Create particle from data in a string
// Expected format (all in double): radius posx posy vx vy 
// prints errors if it couldn't read string
bool part_readData(const char *string);

// ------------
// force/simultaion related
// prints force norm between the two first particles
void particule_force_rendu1();
// update simulation (calc all forces, acceleration, speed and pos) 
// for all particles (don't make locked particles move)
// delta_t is the amount of time the "tick" lasts 
// return false if particle data structure doesn't exist
// (try creating a particle first), or delta_t<0
bool part_nextTick(double delta_t);
// return ID of closest part to a point
int part_closestPart(POINT point);

//return total number of particle
int part_totalNB();

//manages the display of particles
void part_display(void);

//gets datas of particles
double part_get_rayon(int partID);
POINT part_get_center(int partID);
VECTOR part_get_vecteur(int partID);

#endif
