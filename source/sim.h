#ifndef SIM_H
#define SIM_H

#include <stdbool.h>

bool sim_lecture(char mode_de_test[], char filename[]);

void sim_error(char filename[]);
void sim_clean();

#endif
