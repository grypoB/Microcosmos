/* Nom: sim.h
 * Description: module qui lit le fichier en entrée
 * Date: 22.03.2014
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef SIM_H
#define SIM_H

#include <stdbool.h>
#include "geometry.h"

//Defines tab, in which the number of each entity is stored
#define ENTITY_NB 3
#define PART_SLOT 0
#define GEN_SLOT 1
#define BCKH_SLOT 2


// Different mode supported by the simulation
// for more details see the specs of the projects
void sim_error(const char filename[]);
void sim_force(const char filename[]);
void sim_graphic(const char filename[]);
void sim_simulation(char filename[]);

//updates the number of entities
void sim_nbEntities(int elementnb[ENTITY_NB]);

//TODO, documentation
void sim_extremPoints(double *xmin, double *xmax, double *ymin, double *ymax);

//saves the current state of simulation in filename
void sim_save(char filename[]);

//displays entities
void sim_display(void);

//handles calculation for the next step of the simulation
void sim_next_step(void);

// Free memory from all modules accross the simultion
void sim_clean();

#endif


