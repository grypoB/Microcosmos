#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "sim.h"

enum Mode {ERROR, FORCE, INTEGRATION, GRAPHIC, SIMULATION};

int main (int argc, char *argv[])
{ 
    enum Mode mode;
    
    if(strcmp(argv[1], "Error" ) == 0)
    { 
		mode = ERROR;
    }
    else if (strcmp(argv[1], "Force" ) == 0)
    { 
		mode = FORCE;
    }
    else if (strcmp(argv[1], "Integration" ) == 0)
    { 
		mode = INTEGRATION;
    }
    else if (strcmp(argv[1], "Graphic" ) == 0)
    { 
		mode = GRAPHIC;
    }
    else if (strcmp(argv[1], "Simulation" ) == 0)
    { 
		mode = SIMULATION;
    }
    
	if(argc==1)
	{
		//initialise interface graphique //demo.x a revoir
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
			//default: error_msg("invalid argument in main (main.cpp)");
		}
	}
	return EXIT_SUCCESS;
}

