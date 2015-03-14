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

/*
Particles caracterised by : 
- an id
- a lock state (to be able to move or not)
- a radius
- a mass (dependante of radius)
- a center point
- a force, acceleration and speed vector
*/


// ------------
// constructor
// return UNNASIGNED if radius not in [RMIN, RMAX], or speed norm>MAX_VITESSE
// otherwise return the id (>=0) of the particule
int  part_create(double radius, POINT center, VECTOR speed);
// check if given params are valid (see part_create)
// if verbose and if param aren't valid it will call the appropriate error fct
// (sending it the given orgin and id number)
bool part_validParams(double radius, POINT center, VECTOR speed,
                      bool verbose, ERREUR_ORIG origin, int id);


// ------------
// Create particule from data in a string
// Expected format (all in double): radius posx posy vx vy 
// prints errors if it couldn't read string
bool part_readData(const char *string);

// ------------
// force/simultaion related
// prints force norm between the two first particules
void particule_force_rendu1();
#endif
