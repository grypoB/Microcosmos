#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim.c"
#include "geometrie.h"
#include "error.h"

void donnees_generateurs(const char * tab,  int *composant) 
{
	GENERATEUR generateur;
	generateur.nb_de_generateurs = 0; 
	int i = 0;
	char fin;
	
	if(!(sscanf(tab, "%lf", &generateur.nb_de_generateurs))) 
	{
		//error_lect_nb_elements(generateur);
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
	composant = composant + 1;
}
