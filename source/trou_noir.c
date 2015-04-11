/* Nom: trou_noir.c
 * Description: module qui gère les trous noirs
 * Date: 22.03.2015
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

#define NB_COTES 4

typedef struct Trou_noir {

    int id; // unique identifier

    POINT center; // the center point of the black hole

} TROU_NOIR;


static void bckH_draw(void *data);
static TROU_NOIR* newBckH();
static void deleteBckH(void *toDelete);


// list where all black holes are stored
static bool bckHList_initialized = false;
static LIST_HEAD blackHoles = {0};

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
    TROU_NOIR *bckH = newBckH();

    bckH->id = id;
    bckH->center = center;

    #ifdef DEBUG
    printf("BlackHole id no %d created\n", id);
    #endif

    id++;

    return bckH->id;
}

// return the total number of current black hole
int bckH_totalNB() {
    return list_getNbElements(blackHoles);
}


//draws black holes
static void bckH_draw(void *data)
{
    TROU_NOIR *bckH = data;

    if (bckH != NULL) {
       graphic_set_line_width(1.);
       graphic_set_color(GREEN);
       graphic_draw_polygon(NB_COTES, bckH->center);
       graphic_draw_circle(bckH->center, RBLACK, DASH_LINE);
    }
}


//manages the display of black holes
void bckH_display(void)
{
    list_fctToAllElements(blackHoles, bckH_draw);
	#ifdef DEBUG
	printf("bckh_display 1\n");
	#endif
}


void bckH_deleteAll() {
    list_deleteAll(&blackHoles);
}

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

static TROU_NOIR* newBckH() {
    TROU_NOIR *newBckH = malloc(sizeof(TROU_NOIR));

    if (!bckHList_initialized) {
        blackHoles = list_create(deleteBckH, NULL, NULL);
        bckHList_initialized = true;
    }
    
    if (newBckH == NULL) {
        error_msg("Allocation failure in Black hole module\n");
    }

    (void) list_add(&blackHoles, newBckH);

    return newBckH;
}

static void deleteBckH(void *toDelete) {
    TROU_NOIR *bckH = (TROU_NOIR*) toDelete;

    if (bckH != NULL) {
        #if DEBUG
        printf("Freing black hole no %d : %f %f\n", 
                bckH->id, bckH->center.x, bckH->center.y);
        #endif

        free(bckH);
    }
}


void bckH_saveAllData(FILE *file) {
    TROU_NOIR *bckH = NULL;
    
    if (list_goToFirst(&blackHoles) != NULL) {
        do {
            bckH = list_getData(blackHoles, LIST_CURRENT);
            fprintf(file, "%f %f\n" , bckH->center.x, bckH->center.y);
        } while (list_goToNext(&blackHoles) != NULL);
    }
}
