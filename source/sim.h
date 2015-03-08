#ifndef SIM_H
#define SIM_H

#include <stdbool.h>

bool sim_lecture(char filename[]);

void sim_error(char filename[]);
void sim_force(char filename[]);


void sim_integration(char filename[]);
void sim_graphic(char filename[]);
void sim_simulation(char filename[]);


void sim_clean();

#endif
