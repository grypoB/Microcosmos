#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim.c"
#include "geometrie.h"
#include "error.h"

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
