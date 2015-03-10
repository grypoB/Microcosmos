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

void sim_error(char filename[]);
void sim_force(char filename[]);

/*
void sim_integration(char filename[]);
void sim_graphic(char filename[]);
void sim_simulation(char filename[]);
*/


void sim_clean();

#endif
