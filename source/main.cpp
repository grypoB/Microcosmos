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
		case Error: sim_error(argv[2]);
		case Force: sim_force(argv[2]);
		case Integration: sim_integration(argv[2]);
		case Graphic: sim_graphic(argv[2]);
		case Simulation: sim_simulation(argv[2]);
		//default: error_msg("invalid argument in main (main.cpp)");
	}
	return 0;
}

//argv[0] pointe sur une chaîne vide.
//argv[1] mode de test
//argv[2] nom du fichier
