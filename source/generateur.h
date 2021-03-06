/* Nom: generateur.h
 * Description: module qui gère les générateurs
 * Date: 11.06.2015
 * version : 1.1
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */


#ifndef GENERATEUR_H
#define GENERATEUR_H

#include "geometry.h"
#include "linked_list.h"
#include <stdbool.h>

// all generators are stored with a static in .c
// creating one returns a unique ID (used to reference this generator later)
// don't forget to delete afterwards (to clear memory), see gen_deleteAll

/*
Generator (of particles) defined by :
- an id
- the position (point) of the particles it creates
- the radius of the particles it creates
- the speed vector of the particles it creates
*/

// ====================================================================
// Creation of generator
/* Create a generator with given parameter */
int gen_create(double radius, POINT center, VECTOR speed);


// ====================================================================
// Destructions
/* Delete generator with given ID */
bool gen_deleteGen(int genID);
/* Delete all existing black holes */
void gen_deleteAll();

// ====================================================================
// String and file interface
/* Create particle from data in a string */
bool gen_readData(const char *string);
/* Append all the generators to a file */
void gen_saveAllData(FILE *file);

// ====================================================================
// Getters
// return total number of generator
int gen_totalNB(void);
// Append to pHead all generators' center
// Please, do not modify their values (except if you feel lucky)
void gen_getAllCenters(LIST_HEAD *pHead);


// ====================================================================
// Manage simulation
//return ID of closest generator to a point
int gen_closestGen(POINT point, double* dist);
/* Make the generator spawn particles */
void gen_nextTick(double delta_t);

// ====================================================================
// Display all generators
void gen_display(void);

#endif
