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

typedef struct Generateur GENERATEUR;

static GENERATEUR* gen_nextEmptySlot();

struct Generateur {
    int id;

    double radius;
    POINT center;
    VECTOR speed;
};

static GENERATEUR *genTab = NULL;
static int genNB = 0;

bool read_dataGen(const char *string) {
    double param[5] = {0};
    bool returnVal = false;

    if (sscanf(string, "%lf %lf %lf %lf %lf", &param[0], &param[1], &param[2], &param[3], &param[4])==5) {
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
    GENERATEUR *pGen = NULL;

    if (part_validParams(radius, center, speed, true, ERR_GENERAT, id)) {
        pGen = gen_nextEmptySlot();

        pGen->radius = radius;
        pGen->center = center;
        pGen->speed  = speed;

        returnID = id;
        genNB++;

        #ifdef DEBUG
        printf("Gen id no %d created\n", id);
        #endif
    }

    id++;

    return returnID;
}

void gen_deleteAll() {
    if (genTab != NULL) {
        free(genTab);
        genNB = 0;
    }
}

static GENERATEUR* gen_nextEmptySlot() {
    static int genTabSize = 0;
    int i = 0;

    if (genTab==NULL) { // if table doesn't exist
        genTab = malloc(sizeof(GENERATEUR)*GEN_TAB_SIZE);
        if (genTab==NULL) error_msg("MEM allocation failed");
        genTabSize = GEN_TAB_SIZE;
    } else if (genNB == genTabSize) { // if table full, increase its size
        GENERATEUR *newGenTab = malloc(sizeof(GENERATEUR)*genTabSize*TAB_GROWTH_RATIO);
        if (newGenTab==NULL) error_msg("MEM allocation failed");
        for (i=0; i<genNB; i++) { 
            newGenTab[i] = genTab[i];
        }
        free(genTab);
        genTab = newGenTab;
        genTabSize*=2;
    }

    return &genTab[genNB];
}
