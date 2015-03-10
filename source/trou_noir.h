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

bool bckH_readData(const char *string);

// return UNNASIGNED=-1 if create unssucceful
int bckH_create(POINT center);
#endif
