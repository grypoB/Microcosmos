#ifndef TROU_NOIR_H
#define TROU_NOIR_H

#include "geometry.h"

bool read_dataBckH(const char *string);

// return UNNASIGNED=-1 if create unssucceful
int bckH_create(POINT center);
#endif
