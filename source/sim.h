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

#define ENTITY_NB 3
#define PART_SLOT 0
#define GEN_SLOT 1
#define BCKH_SLOT 2

char* sim_write(void);
void sim_display(void);
void sim_next_step(void);

// Different mode supported by the simulation
// for more details see the specs of the projects
void sim_error(const char filename[]);
void sim_force(const char filename[]);
void sim_graphic(const char filename[]);

void sim_nbEntities(int elementnb[3]);
double sim_centre_masse(void);
// Free memory from all modules accross the simultion
void sim_clean();

#endif


