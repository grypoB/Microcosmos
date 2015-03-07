/* Nom: sim.c
 * Description: 4. lire le fichier
 * Date: 25.02.2015
 * version : 1.1
 * auteur : PROG II
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


enum Read_state {NB_GENERATEUR,
                 GENERATEUR,
                 NB_TROU_NOIR,
                 TROU_NOIR,
                 NB_PARTICULE,
                 PARTICULE,
                 FIN,
                 ERROR};

static int read_nbEntities(enum Read_state *state, char *tab);
static bool read_entities(enum Read_state *state, char *tab, int *pCounter, int nb_entities);
static char* file_nextUsefulLine(char tab[], int line_size, FILE *file);


void sim_error(char filename[]) {
    if (sim_lecture(filename)) {
        error_success();
    }

    sim_clean();
}


void sim_clean() {
    #ifdef DEBUG
    printf("Freeing memory from entities\n");
    #endif

    part_deleteAll();
    gen_deleteAll();
}


bool sim_lecture(char filename[])
{
    char tab[BUFFER_SIZE] = {0};
    enum Read_state state = NB_GENERATEUR;
    int nb_entities = 0;
    int counter = 0;
    FILE *file = NULL;

    file = fopen(filename, "r");

    if(file != NULL)
    {
        while(state!=ERROR && state !=FIN)
        {
            if (file_nextUsefulLine(tab, BUFFER_SIZE, file)) {
                #ifdef DEBUG
                printf("Read line (state = %d) : %s", state, tab);
                #endif

                switch (state) {
                    case NB_GENERATEUR:
                    case NB_TROU_NOIR:
                    case NB_PARTICULE:
                        nb_entities = read_nbEntities(&state, tab);
                        counter = 0;
                    break;
                    case GENERATEUR: 
                    case TROU_NOIR:
                    case PARTICULE:
                        (void) read_entities(&state, tab, &counter, nb_entities);
                    break;
                    case FIN:
                    case ERROR:
                    default:
                        error_msg("invalid state in sim lecture (sim_lecture)");
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

}

static int read_nbEntities(enum Read_state *state, char *tab) {
    int nb_entities = 0;
    if (sscanf(tab, "%d", &nb_entities)==1) {
        (*state)++;
    } else {
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



static bool read_entities(enum Read_state *state, char *tab, int *pCounter, int nb_entities) {
    char string[BUFFER_SIZE] = {0};
    bool success = false;
    bool missingSeparator = false;

    if (*pCounter==nb_entities) {
        sscanf(tab, "%s", string); 
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
                    success = read_dataGen(tab);
                }
            break;
            case TROU_NOIR:
                if (missingSeparator) {
                    error_lecture_elements(ERR_TROU_N, ERR_TROP);
                } else {
                    success = read_dataBckH(tab);
                }
            break;
            case PARTICULE:
                if (missingSeparator) {
                    error_lecture_elements(ERR_PARTIC, ERR_TROP);
                } else {
                    success = read_dataPart(tab);
                }
            break;
            
            case NB_GENERATEUR:
            case NB_TROU_NOIR:
            case NB_PARTICULE:
            case FIN:
            case ERROR:
            default :
                printf("state : %d\n", *state);
                error_msg("invalid state in sim lecture (read_entities)");
        }

        if (success) {
            (*pCounter)++;
        } else {
            // error message handled in read_dataXXX()
            *state = ERROR;
        }

    }

    return success;
}


// uses fgets to store in tab the next useful line of a file
// omiting the empty lines and commented ones with '#'
// return the value of fgets (NULL if error, else tab adress)
// Caution : each line of the file should have less than line_size caracters 
static char* file_nextUsefulLine(char tab[], int line_size, FILE *file) {
    int useful = true;
    char *returnVal = NULL;
    int i = 0;

    if (file != NULL) {
        do {
            useful = UNASSIGNED;
            returnVal = fgets(tab, line_size, file); 
            if (returnVal != NULL) {
                for (i=0 ; i<line_size && useful==UNASSIGNED; i++) {
                    if (tab[i]=='\n' || tab[i]=='\r' || tab[i]=='#' || tab[i]=='\0') {
                        useful = false;
                    } else if (tab[i] !=' ') {
                        useful = true;
                    }
                }
            }
        } while (!useful);
    }

    return returnVal;
}
