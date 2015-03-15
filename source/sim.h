/* Nom: sim.h
 * Description: module qui lit le fichier en entrée
 * Date: 08.02.2014
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef SIM_H
#define SIM_H

#include <stdbool.h>

char* sim_ecriture(void);
// Different mode supported by the simulation
// for more details see the specs of the projects
void sim_error(const char filename[]);
void sim_force(const char filename[]);
int* sim_graphic(const char filename[]);

// Free memory from all modules accross the simultion
void sim_clean();

#endif
