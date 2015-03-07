#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "constantes.h"
#include "geometry.h"
#include "error.h"
#include "trou_noir.h"


bool read_dataBckH(const char *string) {
    double param[2] = {0};
    bool returnVal = false;

    if (sscanf(string, "%lf %lf", &param[0], &param[1])==2) {
        if (bckH_create(point_create(param[0], param[1])) != UNASSIGNED) {
            returnVal = true;
        }
    } else {
        error_lecture_elements(ERR_TROU_N, ERR_PAS_ASSEZ);
    }

    return returnVal;
}


int bckH_create(POINT center) {
    return 1;
}
