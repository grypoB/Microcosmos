/* Nom: main.cpp
 * Description: Sous-système de contrôle: responsable de la gestion du dialogue avec l'utilisateur et de l'interface graphique
 * Date: 07.03.2015
 * version : 1.1
 * auteur : PROG II
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "sim.h"
#include "main.h"

enum Mode {ERROR, FORCE, INTEGRATION, GRAPHIC, SIMULATION};

int main (int argc, char *argv[])
{ 
	enum Mode mode;
    read_mode(&mode, argv[1]);  //régler problème du pointeur sur énumération....
    
	if(argc!=3)
	{
		//sim.x [Error|Force|Integration|Graphic|Simulation,nom_fichier]; pas encore vraiment compris ceque je dois faire
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
		}
	}
	return EXIT_SUCCESS;
}

void read_mode(Mode* mode, char *argv[])
{
	if(strcmp(argv[1], "Error" ) == 0) 
    { 
		*mode = ERROR;
    }
    else if (strcmp(argv[1], "Force" ) == 0)
    { 
		*mode = FORCE;
    }
    else if (strcmp(argv[1], "Integration" ) == 0)
    { 
		*mode = INTEGRATION;
    }
    else if (strcmp(argv[1], "Graphic" ) == 0)
    { 
		*mode = GRAPHIC;
    }
    else if (strcmp(argv[1], "Simulation" ) == 0)
    { 
		*mode = SIMULATION;
    }
}

