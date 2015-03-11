/* Nom: trou_noir.h
 * Description: module qui gère les trous noirs
 * Date: 08.02.2014
 * version : 1.0
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef TROU_NOIR_H
#define TROU_NOIR_H

#include "geometry.h"

/*
Black hole defined by :
- a center point
*/

bool bckH_readData(const char *string);

// return UNNASIGNED=-1 if create unssucceful
// otherwise return id of black hole (starting at 0)
int bckH_create(POINT center);
#endif
