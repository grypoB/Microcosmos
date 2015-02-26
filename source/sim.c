/* Nom: sim.c
 * Description: 4. lire le fichier
 * Date: 25.02.2015
 * version : 1.1
 * auteur : PROG II
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void donnees_generateurs();
void donnees_trous_noirs();
void donnees_particules();

int sim()
{
	enum Composants_lecture {GENERATEUR, TROU_NOIR, PARTICULE};
	int composant = GENERATEUR;
	char tab[100];
	FILE * fichier = NULL;
	
	if(!(fichier = fopen ("E*.txt", "r"))) //changer E*.txt
	{
		error_fichier_inexistant();
	}
	
	while(fgets(tab, 100, fichier))
	{
		if ((tab[0]=='#')||(tab[0]=='\n')||(tab[0]=='\r'))
			continue;
		switch(composant)
		{
			case GENERATEUR: donnees_generateurs();
			case TROU_NOIR: donnees_trous_noirs();
			case PARTICULE: donnees_particules();	
		}
		error_success();
	}
	fclose (fichier);
	return 0;
}

