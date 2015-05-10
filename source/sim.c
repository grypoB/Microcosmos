/* Nom: sim.c
 * Description: module qui lit le fichier en entrée
 * Date: 19.04.2015
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"
#include "constantes.h"

#include "sim.h"
#include "generateur.h"
#include "trou_noir.h"
#include "particule.h"
#include "geometry.h"

#define BUFFER_SIZE 100 // size of reading buffer
// separator in input file between entities declarations
#define DATA_SEPARATOR "FIN_LISTE"


// enum to store state of reading automate
// see sim_lecture, read_nbEntities and read_entities
enum Read_state {NB_GENERATEUR,
                 GENERATEUR,
                 NB_TROU_NOIR,
                 TROU_NOIR,
                 NB_PARTICULE,
                 PARTICULE,
                 FIN,
                 ERREUR};
static enum SelectedEntity {PART, GEN, BCKH} selected_entity;
static int selected;

// Reading file
/* read a file and store all entities read into the appropriate module */
static bool sim_lecture(const char filename[]);
/* Centralised way of reading the number of entities in the input file */
static int read_nbEntities(enum Read_state *state, const char *line);
/* centralised way to read an entity value */
static bool read_entities (enum Read_state *state, const char *line,
                           int *pCounter, int nb_entities);
/*parse the empty or commented lines in a file to find the next useful line*/
static char* file_nextUsefulLine(char line[], int line_size, FILE *file);

// ====================================================================
// File managment
/** Different mode supported by the simulation
 * open simulation from file
 * for more details see the specs of the project
 */
void sim_openFile(const char filename[], enum Mode mode)
{
	if (sim_lecture(filename)) 
	{
		switch(mode) 
        {
            case ERROR:       error_success();
                              sim_clean();
            break;
            case FORCE:       particule_force_rendu1();
                              sim_clean();
            break;
            case INTEGRATION: particule_integration_rendu2();
            break;
            // handle GRAPHIC, SIMULATION and DEFAULT the same way
            case GRAPHIC:
            case SIMULATION:
            case DEFAULT:
            break;
            case MODE_UNSET:  printf("Invalid mode\n");
            break;
        }
	}
    else // delete entity which were created if file has errors
    {
        sim_clean();
    }

}

//saves the current state of the simulation in a file which name is given
void sim_save(const char filename[])
{
	FILE *file = fopen(filename, "w");

    if(file != NULL)
    {
        // deleguate to all module;
        
        // gen
        fprintf(file, "%d\n", gen_totalNB());
        gen_saveAllData(file);
		fprintf(file, "%s\n",DATA_SEPARATOR);
        // bckH
        fprintf(file, "%d\n", bckH_totalNB());
        bckH_saveAllData(file);
		fprintf(file, "%s\n",DATA_SEPARATOR);
        // part
        fprintf(file, "%d\n", part_totalNB());
        part_saveAllData(file);
		fprintf(file, "%s\n",DATA_SEPARATOR);

	    fclose(file);
	}
	else printf("File didn't open in %s\n", __func__);
}

// ====================================================================
// Getters : info about the simulation
// get the number of every single entities
void sim_nbEntities(int elementnb[ENTITY_NB])
{
	elementnb[PART_SLOT] = part_totalNB();
	elementnb[GEN_SLOT]  = gen_totalNB();
	elementnb[BCKH_SLOT] = bckH_totalNB();
}

// return the outermost points of the simulation
void sim_extremPoints(double *xmin, double *xmax, double *ymin, double *ymax)
{
    LIST_HEAD centers = list_create(NULL, NULL, NULL);
    POINT *point = NULL;

    // init with default values
    *xmin = 0;
    *xmax = 0;
    *ymin = 0;
    *ymax = 0;

    // retrieve all center point from all entities
    part_getAllCenters(&centers);
    gen_getAllCenters(&centers);
    bckH_getAllCenters(&centers);

    if (list_goToFirst(&centers) != NULL) {

        // initialize with values from points
        point = list_getData(centers, LIST_CURRENT);
        *xmin = point->x;
        *xmax = point->x;
        *ymin = point->y;
        *ymax = point->y;

        while (list_goToNext(&centers) != NULL) {
            point = list_getData(centers, LIST_CURRENT);

            if(point->x < *xmin) *xmin = point->x;
            if(point->x > *xmax) *xmax = point->x;
            if(point->y < *ymin) *ymin = point->y;
            if(point->y > *ymax) *ymax = point->y;
        }
    }

    #ifdef DEBUG
    printf("Extrem points : xmin=%f, xmax=%f, ymin=%f, ymax=%f\n",
		   *xmin, *xmax, *ymin, *ymax);
    #endif

    // free memory
    list_deleteAll(&centers);
}

// ====================================================================
//displays entities
void sim_display(void)
{
	part_display();
	gen_display();
	bckH_display();
}

// ====================================================================
// handles calculation for the next step of the simulation
void sim_next_step(void)
{
    gen_nextTick(DELTA_T);  // create new particles
    part_initTick();
    
    bckH_calcTick();        // caculate all the forces
    part_calcTick();

	part_nextTick(DELTA_T); // update kinematic
    bckH_nextTick();        // let the black holes eat the particles
}



// ====================================================================
// Inputs
// select the closest entity of point (x,y)
void sim_select(double x, double y) {
    
    POINT point = {x , y};

    double dist_gen  = 0;
    double dist_bckh = 0;
    int closestGen  = gen_closestGen(point, &dist_gen);
    int closestBckH = bckh_closestBckH(point, &dist_bckh);
    
    if(part_closestPartOn(point) != UNASSIGNED)
    {
		selected_entity = PART;
		selected = part_closestPartOn(point);
		part_setLock(selected, true);
	}   
	
    else 
    {
		if(closestBckH == UNASSIGNED || (dist_gen) < (dist_bckh))
		{
			selected_entity = GEN;
			selected = closestGen;
		}
	
		else if(closestGen == UNASSIGNED || (dist_gen) > (dist_bckh))
		{
			selected_entity = BCKH;
			selected = closestBckH;
		}
	}

    
    #ifdef DEBUG
    printf("selected type: %d\n", selected_entity);
    printf("closestGen: %d (%f)\n", closestGen, dist_gen); 
    printf("closestBckh: %d (%f)\n", closestBckH, dist_bckh); 
    printf("finally selected: %d\n", selected);
    #endif
    
    printf("%s\n", __func__);
}

// delete the current selection
// in nothing selected, don't do anything
void sim_deleteSelection() {
	
	if (selected!=UNASSIGNED)
	{
		switch(selected_entity)
		{
			case PART: part_deletePart(selected);
			break;
			case GEN:  gen_deleteGen(selected);
			break;
			case BCKH: bckH_deleteBckH(selected);
			break;
		}
	}
    printf("%s\n", __func__);
}

// deselect the current selection
void sim_deselect() {

	if(selected != UNASSIGNED)
	{
		if(selected_entity == PART)
		{
			part_setLock(selected, false);
		}
		selected = UNASSIGNED;
	}
	printf("%s\n", __func__);
}


// ====================================================================
// Free memory from all modules accross the simultion
void sim_clean() {
    #ifdef DEBUG
    printf("Freeing memory from entities\n");
    #endif
	sim_deselect();
	
    part_deleteAll();
    gen_deleteAll();
    bckH_deleteAll();
}


// ====================================================================
// Reading file
/** read a file and store all entities read into the appropriate module
 * return false, if an error occured
 * (ex : file formated wrong, param not in validity domain, etc.)
 * see specs of the project for file syntax
 * print the error in the terminal 
 */
static bool sim_lecture(const char filename[])
{
    char line[BUFFER_SIZE] = {0};
    enum Read_state state = NB_GENERATEUR;
    int nb_entities = 0;
    int counter = 0;
    FILE *file = NULL;

    file = fopen(filename, "r");

    if(file != NULL)
    {
        while(state!=ERREUR && state !=FIN)
        {
            if (file_nextUsefulLine(line, BUFFER_SIZE, file)) {
                #ifdef DEBUG
                printf("Read line (state = %d) : %s", state, line);
                #endif

                switch (state) {
                    case NB_GENERATEUR:
                    case NB_TROU_NOIR:
                    case NB_PARTICULE:
                        nb_entities = read_nbEntities(&state, line);
                        counter = 0;
                    break;
                    case GENERATEUR:
                    case TROU_NOIR:
                    case PARTICULE:
                        (void) read_entities(&state, line, &counter,
											 nb_entities);
                    break;
                    case FIN:
                    case ERREUR:
                    default:
                        error_msg("invalid state in sim lecture"
								  "(sim_lecture)");
                }
            } else {
                error_fichier_incomplet();
                state = ERREUR;
            }
        }

        fclose(file);
        if (state == ERREUR) {
            return false;
        } else {
            return true;
        }
    }
    else
    {
        error_fichier_inexistant();
        return false;
	}
}

/** Centralised way of reading the number of entities in the input file 
 * return the nb_entities read
 * modify the state appropriatly after that
 * print any error detected (then set state=error)
 */
static int read_nbEntities(enum Read_state *state, const char *line) {
    int nb_entities = 0;
    bool success = false;

    if (sscanf(line, "%d", &nb_entities)==1 && nb_entities>=0) {
        (*state)++;
        success = true;
    }

    if (!success) {
        switch (*state) {
            case NB_GENERATEUR:
                error_lect_nb_elements(ERR_GENERAT);
            break;
            case NB_TROU_NOIR:
                error_lect_nb_elements(ERR_TROU_N);
            break;
            case NB_PARTICULE:
                error_lect_nb_elements(ERR_PARTIC);
            break;

            case GENERATEUR:
            case TROU_NOIR:
            case PARTICULE:
            case FIN:
            case ERREUR:
            default :
                error_msg("invalid state in sim lecture (read_nbEntities)");
        }
        *state = ERREUR;
    }

    return nb_entities;
}

/** centralised way to read an entity value
 * update the state (see read_nbEntities) and the counter of entities read
 * return false if an error occured (also print it in the terminal)
 */
static bool read_entities(enum Read_state *state, const char *line,
                          int *pCounter, int nb_entities) {
    char string[BUFFER_SIZE] = {0};
    bool success = false;
    bool missingSeparator = false;

    if (*pCounter==nb_entities) {
        sscanf(line, "%s", string); 
        if(strcmp(DATA_SEPARATOR, string) == 0) { // 0 stands for equality
            (*state)++;
            success = true;
        } else {
            missingSeparator = true;
            // state set to error at the end of switch
        }
    }
    if (!success) {
        switch (*state) {
            case GENERATEUR:
                if (missingSeparator) {
                    error_lecture_elements(ERR_GENERAT, ERR_TROP);
                } else {
                    success = gen_readData(line);
                }
            break;
            case TROU_NOIR:
                if (missingSeparator) {
                    error_lecture_elements(ERR_TROU_N, ERR_TROP);
                } else {
                    success = bckH_readData(line);
                }
            break;
            case PARTICULE:
                if (missingSeparator) {
                    error_lecture_elements(ERR_PARTIC, ERR_TROP);
                } else {
                    success = part_readData(line);
                }
            break;
            
            case NB_GENERATEUR:
            case NB_TROU_NOIR:
            case NB_PARTICULE:
            case FIN:
            case ERREUR:
            default :
                error_msg("invalid state in sim lecture (read_entities)");
        }

        if (success) {
            (*pCounter)++;
        } else {
            // error message handled in ...._readData()
            *state = ERREUR;
        }

    }
    
    return success;
}

/** parse the empty or commented lines in a file to find the next useful line
 * line content stored in char line[], file should have already open
 * Useful line are : none empty and without '#' as first characters
 * return the value fgets returned (line[] address, or NULL if error occured)
 */
static char* file_nextUsefulLine(char line[], int line_size, FILE *file) {
    int useful = true;
    char *returnVal = NULL;
    int i = 0;

    if (file != NULL) {
        do {
            useful = UNASSIGNED;
            returnVal = fgets(line, line_size, file); 

            if (returnVal != NULL) {
                for (i=0 ; i<line_size && useful==UNASSIGNED; i++) {
                    if (line[i]=='\n' || line[i]=='\r' || 
                        line[i]=='#' || line[i]=='\0') {
                        useful = false;
                    } else if (line[i] !=' ') {
                        useful = true;
                    }
                }
            }
        } while (!useful);
    }

    return returnVal;
}

