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

#define GEN_TAB_SIZE MAX_RENDU1

typedef struct Generateur {

    int id; // unique identifier
    
    // params of the particles it creates
    // (see particle module for validity domain of params)
    double radius;
    POINT  center;
    VECTOR speed;

} GENERATEUR;


// tab where all generators are stored
static GENERATEUR genTab[GEN_TAB_SIZE];
// store the number of current generators
static int genNB = 0;

void gen_draw(double radius, POINT center, VECTOR speed);
void gen_display(void);

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

// return the total number of current generator
int gen_totalNB() {
    return genNB;
}

//manages the display of generators
void gen_display(void)
{
	int i;
	for (i=0; i<genNB; i++)
	{
		gen_draw(genTab[i].radius, genTab[i].center, genTab[i].speed);
	}
	#ifdef DEBUG
	printf("gen_display 1\n");
	#endif
}

//draws generators
void gen_draw(double radius, POINT center, VECTOR speed)
{
	graphic_draw_point(center);
	graphic_draw_vector(center, radius, speed);
	graphic_set_line_width(1.);
	graphic_set_color(BLUE);
}

double gen_get_rayon(int partID)
{
	return genTab[partID].radius;
}

POINT gen_get_center(int partID)
{
	POINT centre;
	centre.x = genTab[partID].center.x;
	centre.y = genTab[partID].center.y;
	return centre;
}

VECTOR gen_get_vecteur(int partID)
{
	VECTOR speed;
	speed.x = genTab[partID].speed.x;
	speed.y = genTab[partID].speed.x;
	return speed;
}

