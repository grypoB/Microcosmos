#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "constantes.h"
#include "geometry.h"
#include "error.h"
#include "generateur.h"

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

/*
void donnees_generateurs(const char * tab,  int *composant) 
{
	GENERATEUR generateur;
	generateur.nb_de_generateurs = 0; 
	int i = 0;
	char fin;
	
	if(!(sscanf(tab, "%lf", &generateur.nb_de_generateurs))) 
	{
		error_lect_nb_elements(ERR_GENERAT);
	}
	
	for(i=0; i<generateur.nb_de_generateurs; i++)
	{
		sscanf(tab, "%lf" "%lf" "%lf" "%lf" "%lf", &generateur.rgen, &generateur.posx, &generateur.posy, &generateur.vpi_x, &generateur.vpi_y);
	}	
	
	sscanf(tab, "%c", &fin);
	char fin_entitee[] = "FIN_LISTE";
	if(!(strcmp(fin_entitee, &fin))) 
	{	
		//error_lecture_elements();
	}
	*composant++;
}
*/
