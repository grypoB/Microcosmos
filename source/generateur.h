#ifndef GENERATEUR_H
#define GENERATEUR_H

#include "geometry.h"

bool read_dataGen(const char *string);

// return UNNASIGNED=-1 if create unssucceful
int gen_create(double radius, POINT center, VECTOR speed);
#endif
