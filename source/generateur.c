/* Nom: generateur.c
 * Description: module qui gère les générateurs
 * Date: 17.04.2015
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

// size of center point of generator
#define GEN_RAYON_RATIO 0.1
// size of the line of the vector
#define LINE_WIDTH 2.


typedef struct Generateur {

    int id; // unique identifier
    
    // params of the particles it creates
    // (see particle module for validity domain of params)
    double radius;
    POINT  center;
    VECTOR speed;

} GENERATEUR;


// Graphics
static void gen_draw(void *data);

// to manage data structure
static GENERATEUR* newGen();
static int idGen(void *p_a);
static void deleteGen(void *toDelete);


// ====================================================================
// list where all generators are stored (needs to be initialized)
static bool genList_initialized = false;
static LIST_HEAD generators = {0};


// ====================================================================
// Creation of generator
/** Create a generator with given parameter
 *      see part_create
 * Parameters :
 *  - radius : radius of the generated particle, valide domain [RMIN, RMAX]
 *  - center : position of it's center when created
 *  - speed  : generated particle's speed, validity domain : norm<=MAX_VITESSE
 *             see vector_norm
 * Return values :
 *  - id of generator created (>=0) for future reference
 *  - UNNASIGNED (=-1) if parameters unvalid, prints error in terminal
 *      see error_rayon_partic, error_vitesse_partic
 */
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


// ====================================================================
// Destructions
/** Delete generator with given ID
 * Parameters :
 *  - genID : id of generator to delete
 *             see gen_create
 * Return values :
 *  - if generators was successfully deleted
 *  - if returns false probably already deleted of never existed
 */
bool gen_deleteGen(int genID) {

    if (list_getDataFromId(&generators, genID) == NULL) {
        return false;
    } else { // particule found in list and is the current one
        (void) list_deleteCurrent(&generators);
        return true;
    }

}

/** Delete all existing black holes
 * Note : use with care
 */
void gen_deleteAll() {
    list_deleteAll(&generators);
}


// ====================================================================
// String and file interface
/** Create particle from data in a string
 *      see part_create
 * Parameters :
 *  - string : to parse data from
 *             Expected format (all in double): radius posx posy vx vy
 * Return values :
 *  - if generator was successfully created
 *  - if false, error in string format or parameters value (see gen_create)
 *              and appropriate errors will be displayed in terminal
 */
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

/** Append all the generators to a file
 *      write in the same format as string format in gen_readData
 * Parameters : 
 *  - file : file to append to
 */
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


// ====================================================================
// Getters
// return total number of generator
int gen_totalNB() {
    return list_getNbElements(generators);
}

// Append to pHead all generators' center
// Please, do not modify their values (except if you feel lucky)
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

// ====================================================================
// Manage simulation
//return ID of closest generator to a point
int gen_closestGen(POINT point, double* dist)
{
	int genID = UNASSIGNED;
	double newDist = 0;
    GENERATEUR* current = NULL;
	

	if (list_goToFirst(&generators) != NULL) {
        // init the return to the first generator
        current = list_getData(generators, LIST_CURRENT);
        *dist = point_distance(current->center, point);
        genID = current->id;
        do { // cycle through the rest of the generators
            current = list_getData(generators, LIST_CURRENT);
            newDist = point_distance(current->center, point);

            if (newDist < *dist) {
                *dist = newDist;
                genID = current->id;
            }
        } while (list_goToNext(&generators) != NULL);
    }
	
	return genID;
}


void gen_nextTick(double delta_t) {
    GENERATEUR *gen = NULL;

    if (list_goToFirst(&generators) != NULL) {
        do {
            gen = list_getData(generators, LIST_CURRENT);
            
            //rand !!
            if (randomDouble()<delta_t*NBP && part_closestPartOn(gen->center)==UNASSIGNED) {
                (void) part_create(gen->radius, gen->center, gen->speed);
            }

        } while (list_goToNext(&generators) != NULL);
    }
}

// ====================================================================
// Graphics
// Display all generators
void gen_display(void)
{
    list_fctToAllElements(generators, gen_draw);
}

// Draw a single generator
static void gen_draw(void *data)
{
    GENERATEUR *gen = data;

    if (gen != NULL) {
        graphic_set_line_width(LINE_WIDTH);
        graphic_set_color(AQUA);
        graphic_draw_circle(gen->center, gen->radius*GEN_RAYON_RATIO, DISC);
        graphic_set_color(BLUE);
        graphic_draw_vector(gen->center, gen->speed);
    }
}


// ====================================================================
// utilities functions (managing datas tructure)
// return pointer to an empty slot in the data structure
static GENERATEUR* newGen() {
    GENERATEUR* newGen = malloc(sizeof(GENERATEUR));

    if (!genList_initialized) {
        // TODO add support the other fct
        generators = list_create(deleteGen, NULL, idGen);
        genList_initialized = true;
    }

    if (newGen == NULL) {
        error_msg("Allocation failure in Generateur module\n");
    }

    (void) list_add(&generators, newGen);

    return newGen;
}

void deleteGen(void *toDelete) {
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

// return ID of generateur
static int idGen(void *p_a) {
    int val = UNASSIGNED; 

    if (p_a != NULL) {
        val = ((GENERATEUR*)p_a)->id;
    }

    return val;
}
