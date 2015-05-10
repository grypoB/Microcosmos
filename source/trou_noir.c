/* Nom: trou_noir.c
 * Description: module qui gère les trous noirs
 * Date: 17.04.2015
 * version : 1.0
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
#include "trou_noir.h"
#include "particule.h"

// for drawing of center polygon of the black hole
#define NB_COTES 4
#define RAYON 2
// size of the outer circle line
#define LINE_WIDTH 1.5


typedef struct Trou_noir {

    int id; // unique identifier

    POINT center; // the center point of the black hole

} TROU_NOIR;


// Graphic func
static void bckH_draw(void *data);

// Simulation related
static VECTOR bckH_forceField(POINT p);
static double bckH_calcForceToParticles(TROU_NOIR bckH, POINT p);

// to manage data structure
static TROU_NOIR* newBckH();
static void deleteBckH(void *toDelete);
static int idBckH(void *p_a);


// ====================================================================
// list where all black holes are stored (needs to be initialized)
static bool bckHList_initialized = false;
static LIST_HEAD blackHoles = {0};



// ====================================================================
// Creation of black hole
/** Create a black hole with given parameter
 * Parameters :
 *  - center : position of it's center when created
 * Return values :
 *  - id of black holes created (>=0) for future reference
 */
int bckH_create(POINT center) {
    static int id = 0;
    TROU_NOIR *bckH = newBckH();

    bckH->id = id;
    bckH->center = center;

    #ifdef DEBUG
    printf("BlackHole id no %d created\n", id);
    #endif

    id++;

    return bckH->id;
}


// ====================================================================
// Destructions
/** Delete black holes with given ID
 * Parameters :
 *  - bckHID : id of black hole to delete
 *             see bckH_create
 * Return values :
 *  - if black hole was successfully deleted
 *  - if returns false probably already deleted of never existed
 */
bool bckH_deleteBckH(int bckHID) {

    if (list_getDataFromId(&blackHoles, bckHID) == NULL) {
        return false;
    } else { // black holes found in list and is the current one
        (void) list_deleteCurrent(&blackHoles);
        return true;
    }

}

/** Delete all existing black holes
 * Note : use with care
 */
void bckH_deleteAll() {
    list_deleteAll(&blackHoles);
}


// ====================================================================
// String and file interface
/** Create black hole from data in a string
 *      see bckH_create
 * Parameters :
 *  - string : to parse data from
 *             Expected format (all in double): posx posy
 * Return values :
 *  - if blach hole was successfully created
 *  - if false, error in string format or parameters value (see bckH_create)
 *              and appropriate errors will be displayed in terminal
 */
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

/** Append all the black holes to a file
 *      write in the same format as string format in bckH_readData
 * Parameters : 
 *  - file : file to append to
 */
void bckH_saveAllData(FILE *file) {
    TROU_NOIR *bckH = NULL;
    
    if (list_goToFirst(&blackHoles) != NULL) {
        do {
            bckH = list_getData(blackHoles, LIST_CURRENT);
            fprintf(file, "%f %f\n" , bckH->center.x, bckH->center.y);
        } while (list_goToNext(&blackHoles) != NULL);
    }
}


// ====================================================================
// Getters
// Append to pHead all black holes' center
// Please, do not modify their values (except if you feel lucky)
void bckH_getAllCenters(LIST_HEAD *pHead)
{
    TROU_NOIR *bckH = NULL;

    if (list_goToFirst(&blackHoles) != NULL) {
        do {
            bckH = list_getData(blackHoles, LIST_CURRENT);
            (void) list_add(pHead, &(bckH->center));
        } while (list_goToNext(&blackHoles) != NULL);
    }
}

// return the total number of current black hole
int bckH_totalNB() {
    return list_getNbElements(blackHoles);
}


// ====================================================================
// Manage simulation
//return ID of closest blackhole to a point
int bckh_closestBckH(POINT point, double* dist)
{
	int bckHID = UNASSIGNED;
	double newDist = 0;
	*dist = 0;
    TROU_NOIR* current = NULL;
	
	if (list_goToFirst(&blackHoles) != NULL) {
		*dist = point_distance(current->center, point);
        do {
            current = list_getData(blackHoles, LIST_CURRENT);
            newDist = point_distance(current->center, point);

            if (newDist < *dist) {
                *dist = newDist;
                bckHID = current->id;
            }
        } while (list_goToNext(&blackHoles) != NULL);
    }
	
	return bckHID;
}


// ====================================================================
// Drawings
//Display all black holes 
void bckH_display(void)
{
    list_fctToAllElements(blackHoles, bckH_draw);
}

//draws a single black hole
static void bckH_draw(void *data)
{
    TROU_NOIR *bckH = data;

    if (bckH != NULL) {
       graphic_set_line_width(LINE_WIDTH);
       graphic_set_color(GREEN);
       graphic_draw_polygon(bckH->center, NB_COTES, RAYON);
       graphic_draw_circle(bckH->center, RBLACK, DASH_LINE);
    }
}

// ====================================================================
// Simulation update fct
/* Apply forces to all particles */
void bckH_calcTick() {
    // apply forces to particles
    part_applyForceField(bckH_forceField);
}


/* Destroy all particles too close from the black holes */
void bckH_nextTick() {
    // eat those particle which were a bit too close
    TROU_NOIR *bckH = NULL;
    int partID = UNASSIGNED;

    if (list_goToFirst(&blackHoles) != NULL) {
        do {
            bckH = list_getData(blackHoles, LIST_CURRENT);

            while ((partID=part_closestPartOn(bckH->center)) != UNASSIGNED) {
                part_deletePart(partID);
            }

        } while (list_goToNext(&blackHoles) != NULL);
    }
}

/* Calc the force all black holes apply on a point p */
static VECTOR bckH_forceField(POINT p) {
    TROU_NOIR *bckH = NULL;
    VECTOR force = vector_null();
    double force_norm = 0;

    if (list_goToFirst(&blackHoles) != NULL) {
        do {
            bckH = list_getData(blackHoles, LIST_CURRENT);

            force_norm  = bckH_calcForceToParticles(*bckH, p);
            force = vector_sum(force,
                               vector_fitLine(p, bckH->center, force_norm));

        } while (list_goToNext(&blackHoles) != NULL);
    }

    return force;
}

/* Return the norm of the force a given black hole exerce on a point */
static double bckH_calcForceToParticles(TROU_NOIR bckH, POINT p) {
    double force_norm = FBLACK_MIN;
    double dist = point_distance(bckH.center, p);
    
    if (dist<RBLACK) {
        force_norm += linear_interpolation(dist, 0, FBLACK, RBLACK, 0);
    }

    return force_norm;
}

// ====================================================================
// utilities functions (managing datas tructure)
// return pointer to an empty slot in the data structure
static TROU_NOIR* newBckH() {
    TROU_NOIR *newBckH = malloc(sizeof(TROU_NOIR));

    if (!bckHList_initialized) {
        blackHoles = list_create(deleteBckH, NULL, idBckH);
        bckHList_initialized = true;
    }
    
    if (newBckH == NULL) {
        error_msg("Allocation failure in Black hole module\n");
    }

    (void) list_add(&blackHoles, newBckH);

    return newBckH;
}

void deleteBckH(void *toDelete) {
    TROU_NOIR *bckH = (TROU_NOIR*) toDelete;

    if (bckH != NULL) {
        #if DEBUG
        printf("Freing black hole no %d : %f %f\n", 
                bckH->id, bckH->center.x, bckH->center.y);
        #endif

        free(bckH);
    }
}

// return ID of black holes
static int idBckH(void *p_a) {
    int val = UNASSIGNED; 

    if (p_a != NULL) {
        val = ((TROU_NOIR*)p_a)->id;
    }

    return val;
}
