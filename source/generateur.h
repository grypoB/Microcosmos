/* Nom: generateur.h
 * Description: module qui gère les générateurs
 * Date: 08.02.2014
 * version : 1.1
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */


#ifndef GENERATEUR_H
#define GENERATEUR_H

#include "geometry.h"

bool gen_readData(const char *string);

// return UNNASIGNED=-1 if create unssucceful
int gen_create(double radius, POINT center, VECTOR speed);
void gen_deleteAll();
#endif
