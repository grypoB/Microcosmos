#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "constantes.h"
#include "geometry.h"
#include "error.h"
#include "trou_noir.h"


bool read_dataBckH(const char *string) {
    double param[2] = {0};
    bool returnVal = false;

    if (sscanf(string, "%lf %lf", &param[0], &param[1])==2) {
        if (bckH_create(point_create(param[0], param[1])) != UNASSIGNED) {
            returnVal = true;
        }
    } else {
        error_lecture_elements(ERR_TROU_N, ERR_PAS_ASSEZ);
    }

    // TODO : handle the string  and to create a black hole call :
    //bckH_create(POINT center);
    // don't forget to call appropriate error if sscanf unnssuscecful
    // follow model of read_dataPart()

    return returnVal;
}

/*
void donnees_trous_noirs(const char * tab,  int *composant)
{
	TROU_NOIR trou_noir;
	trou_noir.nb_de_trous_noirs = 0;
	int i = 0;
	char fin;
	
	if(!(sscanf(tab, "%lf", &trou_noir.nb_de_trous_noirs)))
	{
		//error_lect_nb_elements(trou_noir);
	}
	
	for(i=0; i<trou_noir.nb_de_trous_noirs; i++)
	{
		sscanf(tab, "%lf" "%lf", &trou_noir.posx, &trou_noir.posy);
	}	
	
	sscanf(tab, "%c", &fin);
	char fin_entitee[] = "FIN_LISTE";
	if(!(strcmp(fin_entitee, &fin))) 
	{	
		//error_lecture_elements();
	}
	composant = composant + 1;
}
*/
