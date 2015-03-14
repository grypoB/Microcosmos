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

/*
Generator (of particles) defined by :
- an id
- the position (point) of the particles it creates
- the radius of the particles it creates
- the speed vector of the particles it creates
*/

// from an input string, creates a particule generator
// Expected format (all in double): radius posx posy vx vy 
// prints errors if it couldn't read string
bool gen_readData(const char *string);

// return UNNASIGNED=-1 if create unssucceful
// otherwise return the id of the generator
// See particule module for params validity domain
int gen_create(double radius, POINT center, VECTOR speed);
#endif
