/* Nom: trou_noir.h
 * Description: module qui gère les trous noirs
 * Date: 17.04.2015
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef TROU_NOIR_H
#define TROU_NOIR_H

#include "geometry.h"
#include "linked_list.h"
#include <stdbool.h>

// all black holes are stored with a static in .c
// creating one returns a unique ID (used to reference this black hole later)
// don't forget to delete afterwards (to clear memory), see bckH_deleteAll

/*
Black hole defined by :
- an id
- a center point
*/

// ====================================================================
// Creation of black hole
/* Create a black hole with given parameter */
int bckH_create(POINT center);


// ====================================================================
// String and file interface
/* Create black hole from data in a string */
bool bckH_readData(const char *string);

/* Append all the black holes to a file */
void bckH_saveAllData(FILE *file);


// ====================================================================
// Destructions
/** Delete all existing black holes
 * Note : use with care
 */
void bckH_deleteAll();


// ====================================================================
// Getters
//return total number of black holes
int bckH_totalNB();
// Append to pHead all particles' center
// Please, do not modify their values (except if you feel lucky)
void bckH_getAllCenters(LIST_HEAD *pHead);


// ====================================================================
// Display all black holes 
void bckH_display(void);

#endif
