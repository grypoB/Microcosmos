/* Nom: generateur.c
 * Description: module qui gère les générateurs
 * Date: 08.02.2014
 * version : 1.1
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "constantes.h"
#include "geometry.h"
#include "error.h"
#include "generateur.h"
#include "particule.h"

#define GEN_TAB_SIZE MAX_RENDU1

typedef struct Generateur {
    int id;

    double radius;
    POINT  center;
    VECTOR speed;
} GENERATEUR;


static GENERATEUR* gen_nextEmptySlot();


static GENERATEUR genTab[GEN_TAB_SIZE];
static int genNB = 0;


// from an input string, creates a particule generator
// Expected format (all in double): radius posx posy vx vy 
// prints errors if it couldn't read string
bool gen_readData(const char *string) {
    double param[5] = {0};
    bool returnVal = false;

    if (sscanf(string, "%lf %lf %lf %lf %lf", &param[0], &param[1], &param[2],
                                              &param[3], &param[4])==5) {
        if (gen_create(param[0], point_create(param[1], param[2]),
            vector_create(param[3], param[4])) != UNASSIGNED) {
            returnVal = true;
        }
    } else {
        error_lecture_elements(ERR_GENERAT, ERR_PAS_ASSEZ);
    }

    return returnVal;
}


int gen_create(double radius, POINT center, VECTOR speed) {
    static int id = 0;
    int returnID = UNASSIGNED;

    if (part_validParams(radius, center, speed, true, ERR_GENERAT, id)) {
        if (genNB < GEN_TAB_SIZE) {
            genTab[genNB].radius = radius;
            genTab[genNB].center = center;
            genTab[genNB].speed = speed;

            returnID = id;
            genNB++;

            #ifdef DEBUG
            printf("Gen id no %d created\n", id);
            #endif
        } else {
            error_msg("Error creating generator : not enough memory");
        }
    }

    id++;

    return returnID;
}
