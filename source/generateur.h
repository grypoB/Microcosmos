/* Nom: generateur.h
 * Description: module qui gère les générateurs
 * Date: 22.03.2015
 * version : 1.1
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */


#ifndef GENERATEUR_H
#define GENERATEUR_H

#include "geometry.h"
#include "linked_list.h"
#include <stdbool.h>

/*
Generator (of particles) defined by :
- an id
- the position (point) of the particles it creates
- the radius of the particles it creates
- the speed vector of the particles it creates
*/



// from an input string, creates a particle generator
// Expected format (all in double): radius posx posy vx vy 
// prints errors if it couldn't read string
bool gen_readData(const char *string);
void gen_saveAllData(FILE *file);

// return UNNASIGNED=-1 if create unssucceful
// otherwise return the id of the generator
// See particle module for params validity domain
int gen_create(double radius, POINT center, VECTOR speed);

//return total number of generator
int gen_totalNB(void);
void gen_getAllCenters(LIST_HEAD *pHead);

//manages the displayof the generators
void gen_display(void);

//gets datas of generators 
void gen_deleteAll();

#endif
