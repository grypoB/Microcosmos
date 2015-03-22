/* Nom: trou_noir.h
 * Description: module qui gère les trous noirs
 * Date: 22.03.2015
 * version : 1.0
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef TROU_NOIR_H
#define TROU_NOIR_H

#include "geometry.h"
#include <stdbool.h>

/*
Black hole defined by :
- an id
- a center point
*/

// from an input string, creates a black hole
// Expected format (all in double): posx posy
// prints errors if it couldn't read string
bool bckH_readData(const char *string);

// return UNNASIGNED=-1 if create unssucceful
// otherwise return id of black hole (starting at 0)
int bckH_create(POINT center);

//return total number of black hole
int bckH_totalNB();

//manages the display of black holes 
void bckH_display(void);

//saves the number of black holes and their position in filename 
void bckH_save(char filename[]);

#endif
