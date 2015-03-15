/* Nom: trou_noir.c
 * Description: module qui gère les trous noirs
 * Date: 08.02.2014
 * version : 1.0
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "constantes.h"
#include "graphic.h"
#include "geometry.h"
#include "error.h"
#include "trou_noir.h"

// size of the tab to store the black holes in
#define BCKH_TAB_SIZE MAX_RENDU1


typedef struct Trou_noir {

    int id; // unique identifier

    POINT center; // the center point of the black hole

} TROU_NOIR;


// tab where all black holes are stored
static TROU_NOIR bckHTab[BCKH_TAB_SIZE];
// store the number of current black holes
static int bckHNB = 0;


// from an input string, creates a black hole
// Expected format (all in double): posx posy
// prints errors if it couldn't read string
bool bckH_readData(const char *string) {
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


// return UNNASIGNED=-1 if create unssucceful
// otherwise return id of black hole (starting at 0)
int bckH_create(POINT center) {
    static int id = 0;
    int returnID = UNASSIGNED;

    if (bckHNB < BCKH_TAB_SIZE) {
        bckHTab[bckHNB].center = center;

        returnID = id;

        bckHNB++;

        #ifdef DEBUG
        printf("BlackHole id no %d created\n", id);
        #endif

    } else {
        error_msg("Error creating black hole : not enough memory");
    }

    id++;

    return returnID;
}

// return the total number of current black hole
int bckH_totalNB() {
    return bckHNB;
}

void BCKh_draw(POINT center)
{
	//graphic_set_line_width(1.);
	//graphic_set_color3f(0.2, 0.8,1.0);
	graphic_draw_bckH(center);
}
