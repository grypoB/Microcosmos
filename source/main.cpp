#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "sim.h"
#include "particule.h"

int main (int argc, char *argv[])
{
	enum argument {Error, Force, Integration, Graphic, Simulation};
	
	switch(*argv[1]) 
	{
		sim_lecture(argv[1], argv[2]);
		case Error: 
		case Force: particule_force_rendu1();
		case Integration:
		case Graphic:
		case Simulation:   ;
		//default: error_msg("invalid argument in main (main.cpp)");
	}
	return 0;
}

//argv[0] doit alors pointer sur une chaîne vide.
//argv[1] mode de test
//argv[2] nom du fichier
