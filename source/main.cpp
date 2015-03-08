/* Nom: main.cpp
 * Description: Sous-système de contrôle: responsable de la gestion du dialogue avec l'utilisateur et de l'interface graphique
 * Date: 07.03.2015
 * version : 1.1
 * auteur : PROG II
 */

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "sim.h"
#include "main.h"
}

int main (int argc, char *argv[])
{ 
	enum Mode mode;
    mode = read_mode(argv[1]);
    
	if(argc!=3)
	{
		printf("syntaxe attendue : sim.x [Error|Force|Integration|Graphic|Simulation,nom_fichier]\n");
	}
	else
	{
		switch(mode) 
		{
			case ERROR: sim_error(argv[2]);
			break;
			case FORCE: sim_force(argv[2]);
			break;
			case INTEGRATION: sim_integration(argv[2]);
			break;
			case GRAPHIC: sim_graphic(argv[2]);
			break;
			case SIMULATION: sim_simulation(argv[2]);
			break;
			case MODE_UNSET: printf("syntaxe attendue : sim.x [Error|Force|Integration|Graphic|Simulation,nom_fichier]\n");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

MODE read_mode(char string[])
{
	MODE mode;
	if(strcmp(string, "Error" ) == 0) 
    { 
		mode = ERROR;
    }
    else if (strcmp(string, "Force" ) == 0)
    { 
		mode = FORCE;
    }
    else if (strcmp(string, "Integration" ) == 0)
    { 
		mode = INTEGRATION;
    }
    else if (strcmp(string, "Graphic" ) == 0)
    { 
		mode = GRAPHIC;
    }
    else if (strcmp(string, "Simulation" ) == 0)
    { 
		mode = SIMULATION;
    }
    else 
    { 
		mode = MODE_UNSET;
    }
    return mode;
}

