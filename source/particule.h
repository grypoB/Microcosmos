/* Nom: particule.h
 * Description: module qui gère les particules
 * Date: 08.02.2014
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef PARTICULE_H
#define PARTICULE_H

#include "geometry.h"
#include "error.h"


// all particules are stored with a static in .c
// creating one returns a unique ID (used to reference this particule later)
// don't forget to delete afterwards (to clear memory)


// ------------
// constructor
// return UNNASIGNED if radius not in [RMIN, RMAX], or speed norm > MAX_VITESSE
// otherwise return the id (>=0) of the particule
int  part_create(double radius, POINT center, VECTOR speed);
// check if given params are valid (see part_create)
// if verbose and if param aren't valid
// it will call the appropriate error fct sending it the given orgin and id number
bool part_validParams(double radius, POINT center, VECTOR speed,
                      bool verbose, ERREUR_ORIG origin, int id);


// ------------
// destructors
// return false if particule wasn't found
bool part_deletePart(int partID);
// delete all particules, use with caution
void part_deleteAll();

// setter, return true if set was successful (i.e. particle was found)
// locked particules can't move (see part_nextTick)
bool part_setLock  (int partID, bool lock);


// ------------
// Create particule from data in a string
// Expected format (all in double): radius posx posy vx vy 
bool part_readData(const char *string);

// ------------
// force/simultaion related
// prints force norm between the two first particules
void particule_force_rendu1();
// update simulation (calc all forces, acceleration, speed and pos) 
// for all particules (don't make locked particules move)
// delta_t is the amount of time the "tick" lasts 
// return false if particule data structure doesn't exist
// (try creating a particule first), or delta_t<0
bool part_nextTick(double delta_t);
// return ID of closest part to a point
int part_closestPart(POINT point);
int part_totalNB();
#endif
