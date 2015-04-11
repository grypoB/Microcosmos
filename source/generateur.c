/* Nom: generateur.c
 * Description: module qui gère les générateurs
 * Date: 22.03.2015
 * version : 1.1
 * responsable du module : Alexandre Devienne
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
#include "generateur.h"
#include "particule.h"

#define GEN_RAYON 2

typedef struct Generateur {

    int id; // unique identifier
    
    // params of the particles it creates
    // (see particle module for validity domain of params)
    double radius;
    POINT  center;
    VECTOR speed;

} GENERATEUR;


static void gen_draw(void *data);

static void deleteGen(void *toDelete);
static GENERATEUR* newGen();

// list where all generators are stored
static bool genList_initialized = false;
static LIST_HEAD generators = {0};

// from an input string, creates a particle generator
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


// return UNNASIGNED=-1 if create unssucceful
// otherwise return the id of the generator
// See particle module for params validity domain
int gen_create(double radius, POINT center, VECTOR speed) {
    static int id = 0;
    int returnID = UNASSIGNED;
    GENERATEUR *gen = NULL;


    if (part_validParams(radius, center, speed, true, ERR_GENERAT, id)) {
        gen = newGen();

        gen->id = id;

        gen->radius = radius;
        gen->center = center;
        gen->speed = speed;

        returnID = id;

        #ifdef DEBUG
        printf("Gen id no %d created\n", id);
        #endif
    }

    id++;

    return returnID;
}

// return the total number of current generator
int gen_totalNB() {
    return list_getNbElements(generators);
}

void gen_getAllCenters(LIST_HEAD *pHead)
{
    GENERATEUR *gen = NULL;

    if (list_goToFirst(&generators) != NULL) {
        do {
            gen = list_getData(generators, LIST_CURRENT);
            (void) list_add(pHead, &(gen->center));
        } while (list_goToNext(&generators) != NULL);
    }
}


void gen_deleteAll() {
    list_deleteAll(&generators);
}

//draws generators
static void gen_draw(void *data)
{
    GENERATEUR *gen = data;

    if (gen != NULL) {
        graphic_set_line_width(2.);
        graphic_set_color(BLUE);
        graphic_draw_circle(gen->center, GEN_RAYON, DISC);
        graphic_draw_vector(gen->center, gen->speed);
    }
}

//manages the display of generators
void gen_display(void)
{
    list_fctToAllElements(generators, gen_draw);
}

static GENERATEUR* newGen() {
    GENERATEUR* newGen = malloc(sizeof(GENERATEUR));

    if (!genList_initialized) {
        // TODO add support the other fct
        generators = list_create(deleteGen, NULL, NULL);
        genList_initialized = true;
    }

    if (newGen == NULL) {
        error_msg("Allocation failure in Generateur module\n");
    }

    (void) list_add(&generators, newGen);

    return newGen;
}

static void deleteGen(void *toDelete) {
    GENERATEUR *gen = (GENERATEUR*) toDelete;

    if (gen != NULL) {
        #if DEBUG
        printf("Freing generator no %d : %f %f %f %f %f\n", 
                gen->id, gen->radius,
                gen->center.x, gen->center.y,
                gen->speed.x, gen->speed.y);
        #endif

        free(gen);
    }
}

void gen_saveAllData(FILE *file) {
    GENERATEUR *gen = NULL;

    if (list_goToFirst(&generators) != NULL) {
        do {
            gen = list_getData(generators, LIST_CURRENT);
            fprintf(file, "%f %f %f %f %f\n" , gen->radius,
                    gen->center.x, gen->center.y,
                    gen->speed.x,  gen->speed.y);
        } while (list_goToNext(&generators) != NULL);
    }
}
