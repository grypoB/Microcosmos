/* Nom: particule.h
 * Description: module qui gère les particules
 * Date: 15.04.2015
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef PARTICULE_H
#define PARTICULE_H

#include "geometry.h"
#include "error.h"
#include "linked_list.h"
#include <stdbool.h>

// all particles are stored with a static in .c
// creating one returns a unique ID (used to reference this particle later)
// don't forget to delete afterwards (to clear memory), see part_deleteAll

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
// Creations
/* Create a particle with given parameter */
int  part_create(double radius, POINT center, VECTOR speed);
/* Checks if arguments are valid for the creation of a particle */
bool part_validParams(double radius, POINT center, VECTOR speed,
                      bool verbose, ERREUR_ORIG origin, int id);


// ------------
// Destruction
/* Delete particle with given ID */
bool part_deletePart(int partID);
/* Delete all existing particles */
void part_deleteAll();

// ----------
// Setters/Getters
/* Lock a given particle in place (or unlock) */
bool part_setLock(int partID, bool lock);
// Append to pHead all particles' center
// Please, do not modify their values (except if you feel lucky)
void part_getAllCenters(LIST_HEAD *pHead);
//Return total number of existing particle
int part_totalNB();

// ------------
// String and file interface
/* Create particle from data in a string */
bool part_readData(const char *string);
/* Append all the particles to a file */
void part_saveAllData(FILE *file);

// ------------
// Force/Integration related (see specs)
/* prints force norm between the two first particles */
void particule_force_rendu1();
/* prints status of first particle after one tick of simulation */
void particule_integration_rendu2();


// ------------
// Manage simulation
/* Init the partciles for the next tick calculation */
void part_initTick();
/* Calculation for the particles next tick */
void part_calcTick();
/* Update kinematic of all particles to finish the next tick*/
void part_nextTick(double delta_t);

/* Apply a force field to all particles */
void part_applyForceField(VECTOR (*forceFieldAt) (POINT p));
/* return ID of closest part to a point, which it overlaps */
int part_closestPartOn(POINT point);
// Display all particles
void part_display(void);


#endif
