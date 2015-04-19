/* Nom: sim.h
 * Description: module qui lit le fichier en entrée
 * Date: 19.04.2015
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef SIM_H
#define SIM_H

#include <stdbool.h>

//Defines tab, in which the number of each entity is stored
// see sim_nbEntities
#define ENTITY_NB 3
#define PART_SLOT 0
#define GEN_SLOT 1
#define BCKH_SLOT 2

// Mode of the simulation to be ran on, see specs sheet for details
typedef enum Mode {ERROR, FORCE, INTEGRATION, GRAPHIC, SIMULATION, 
				   DEFAULT, MODE_UNSET} MODE;

// ====================================================================
// File management
/** Different mode supported by the simulation
 * open simulation fron file
 * for more details see the specs of the projects
 */
void sim_openFile(const char filename[], enum Mode mode);
// saves the current state of simulation in filename
void sim_save(const char filename[]);

// ====================================================================
// Getters : info about the simulation
// get the number of every single entities
void sim_nbEntities(int elementnb[ENTITY_NB]);
//return the outermost points of the simulation
void sim_extremPoints(double *xmin, double *xmax, double *ymin, double *ymax);

// ====================================================================
//displays entities
void sim_display(void);

// ====================================================================
// handles calculation for the next step of the simulation
void sim_next_step(void);

// ====================================================================
// Inputs
void sim_select(double x, double y);
void sim_deleteSelection();
void sim_deselect();

// ====================================================================
// Free memory from all modules accross the simultion
void sim_clean();

#endif


