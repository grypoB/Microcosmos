#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geometry.h"
#include "error.h"
#include "generateur.h"

bool read_dataGen(const char *string) {
    double param[5] = {0};
    bool returnVal = false;
    
    // TODO : handle the string  and to create a generator call :
    //gen_create(double radius, POINT center, VECTOR speed);
    // don't forget to call appropriate error if sscanf unnssuscecful
    // follow model of read_dataPart()

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
