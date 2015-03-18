/* Nom: sim.c
 * Description: module qui lit le fichier en entrée
 * Date: 08.02.2014
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

#define BUFFER_SIZE 100 // size of reading buffer
// separator in input file between enity declarations
#define DATA_SEPARATOR "FIN_LISTE"

// enum to store state of reading automate
enum Read_state {NB_GENERATEUR,
                 GENERATEUR,
                 NB_TROU_NOIR,
                 TROU_NOIR,
                 NB_PARTICULE,
                 PARTICULE,
                 FIN,
                 ERROR};


// read a file and store all entities read into the appropriate module
// return false, if an error occured
// (ex : file formated wrong, param not in validity domain, etc.)
// see specs of the project for file syntax
// print the error in the terminal 
static bool sim_lecture(const char filename[]);

// Centralised way of reading the number of entities in the input file 
// return the nb_entities read
// modify the state appropriatly after that
// print any error detected (then set state=error)
static int read_nbEntities(enum Read_state *state, const char *line);
// centralised way to read an entity value
// update the state (see read_nbEntities) and the counter of entities read
// return false if an error occured (also print it in the terminal)
static bool read_entities (enum Read_state *state, const char *line,
                           int *pCounter, int nb_entities);
                           
// parse the empty or commented lines in a file to find the next useful line
// line content stored in char line[], file should have already open
// Useful line are : none empty and without '#' as first characters
// return the value fgets returned (line[] address, or NULL if error occured)
static char* file_nextUsefulLine(char line[], int line_size, FILE *file);


// Mode Error, called from main.
// Input : the file to read the entities form
void sim_error(const char filename[]) {
    if (sim_lecture(filename)) {
        error_success();
    }
	printf("sim_error\n");  //stub
    sim_clean();
}

// Mode Force, called from main.
// Input : file to read the entities from
void sim_force(const char filename[]) {
    if (sim_lecture(filename)) {
        particule_force_rendu1();
    } else {
        error_msg("Couldn't open simulation, input file has errors");
    }
	printf("sim_force\n");  //stub
    sim_clean();
}

//Mode Graphic, called from main.
// Input : the file to read the entities form
void sim_graphic(const char filename[])
{
	if (sim_lecture(filename)) {
		//
    } else {
        error_msg("Couldn't open simulation, input file has errors");
    }
	printf("sim_graphic\n");  //stub
}

// Free memory from all modules accross the simultion
void sim_clean() {
    #ifdef DEBUG
    printf("Freeing memory from entities\n");
    #endif

    part_deleteAll();
    //gen_deleteAll();
    //bckH_deleteAll();
}


// ----------
// read a file and store all entities read into the appropriate module
// return false, if an error occured
// (ex : file formated wrong, param not in validity domain, etc.)
// print the error in the terminal 
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
        while(state!=ERROR && state !=FIN)
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
                    case ERROR:
                    default:
                        error_msg("invalid state in sim lecture"
								  "(sim_lecture)");
                }
            } else {
                error_fichier_incomplet();
                state = ERROR;
            }
        }

        fclose(file);
        if (state == ERROR) {
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
	printf("sim_lecture\n");  //stub
}


// Centralised way of reading the number of entities in the input file 
// return the nb_entities read
// modify the state appropriatly after that
// print any error detected (then set state=error)
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
            case ERROR:
            default :
                error_msg("invalid state in sim lecture (read_nbEntities)");
        }
        *state = ERROR;
    }

    return nb_entities;
}


// centralised way to read an entity value
// call appropriate module to create the entity
// update the state and the counter of entities read
// return false if an error occured (also print it in the terminal)
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
            case ERROR:
            default :
                error_msg("invalid state in sim lecture (read_entities)");
        }

        if (success) {
            (*pCounter)++;
        } else {
            // error message handled in ...._readData()
            *state = ERROR;
        }

    }
    
    return success;
}



// ---------
// parse the empty or commented lines in a file to find the next useful line
// line content stored in char line[], file should have already open
// Useful line are : none empty and without '#' as first characters
// return the value fgets returned (line[] address, or NULL if error occured)
// Caution : each line of the file should have less than line_size caracters
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

void sim_display(void)				//pas void mais doit recevoir les infos
{
	part_display();
	gen_display();
	bckH_display();
}

void sim_nbEntities(int elementnb[3])
{
	elementnb[PART_SLOT] = part_totalNB();
	elementnb[GEN_SLOT] = gen_totalNB();
	elementnb[BCKH_SLOT] = bckH_totalNB();
	printf("sim_element\n");  //stub
}

double sim_centre_masse(void)                       //TODO
{
	double cm = 0;
	return cm;
}

/*char* sim_write(void)						//A CHANGER SUREMENT AVEC ALLOCATION DYNAMIQUE (VALEURS ALÉATOIRES)
{
	//appeler chaque module pour mettre info
	* //fprintf
	int i;
	char* file[200];
	
	*file[0] = gen_totalNB();
	for(i=0, i<gen_totalNB(), i++);
	{
		//rayon POINT(center) VECTOR
	}
	*file[8] = "FIN_LISTE";
	
	*file[10] = bckH_totalNB();
	for(i=0, i<bckH_totalNB(), i++);
	{
		//POINT(center)
	}
	*file[18] = "FIN_LISTE";
	
	*file[20] = part_totalNB();
	for(i=0, i<part_totalNB(), i++);
	{
		//rayon POINT(center) VECTOR(speed)
	}
	*file[28] = "FIN_LISTE";
	
	printf("sim_write\n");  //stub
	* 
	return *file;
}
* */

