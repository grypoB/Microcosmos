/* Nom: particule.c
 * Description: module qui gère les particules
 * Date: 15.04.2015
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "constantes.h"
#include "graphic.h"
#include "particule.h"
#include "linked_list.h"


typedef struct Particule {

    bool locked; // a locked particle cannot move
    int id; // the unique identifier of a particle

    double radius; // must be included in [RMIN, RMAX]
    double mass; // depends of the radius (see init_mass)

    POINT center; // position of the center of the particle

    VECTOR speed; // its norm must be below 10
    VECTOR force;
    VECTOR acceleration;

} PARTICULE;


// Init
static void part_initMass(PARTICULE *part);

// Simulation related
static void part_updateAllForces();
static void updateKinematic(PARTICULE *part, double delta_t);
static void initForceVec (void *data);
static void part_interact(void *dataA, void *dataB);
static double part_calcForce (PARTICULE *p1, PARTICULE *p2, double distance);
static void   part_applyForce(PARTICULE *p1, PARTICULE *p2, double distance,
                                                           double force_norm);

// Graphic functions
static void part_draw(void *pData);
static void part_setColor(VECTOR speed);

// to manage data structure
static PARTICULE* newPart();
static PARTICULE* part_findPart(int partID);
static void deletePart(void *toDelete);
static int sortPart(void *p_a, void *p_b);
static int idPart(void *p_a);


// ====================================================================
// list to store all the particle in (needs to be initialized)
static bool partList_initialized = false;
static LIST_HEAD particles = {0};



// ====================================================================
// Creation of particle
/** Create a particle with given parameter
 * Parameters :
 *  - radius : radius of the particle, valide domain [RMIN, RMAX]
 *  - center : position of it's center when created
 *  - speed  : its speed, validity domain : norm<=MAX_VITESSE
 *             see vector_norm
 * Return values :
 *  - id of particles created (>=0) for future reference
 *  - UNNASIGNED (=-1) if parameters unvalid, prints error in terminal
 *      see error_rayon_partic, error_vitesse_partic
 */
int part_create(double radius, POINT center, VECTOR speed) {
    static int id = 0; // static counter (for unique identifier)
    int returnID = UNASSIGNED;
    PARTICULE *pPart = NULL;

    if (part_validParams(radius, center, speed, true, ERR_PARTIC, id)) {
        pPart = newPart();

        pPart->locked = false;
        pPart->id = id;
        pPart->radius = radius;
        part_initMass(pPart);
        pPart->center = center;
        pPart->speed = speed;
        pPart->force = vector_null();
        pPart->acceleration = vector_null();

        returnID = id;

        #ifdef DEBUG
        printf("Part id no %d created\n", id);
        #endif
    }

    id++;

    return returnID;
}

/** Checks if arguments are valid for the creation of a particle
 *      see part_create
 * Parameters :
 *  - radius, center, speed : see part_create
 *  - verbose    : if errors should be displayed in terminal
 *  - origin, id : argument sent to error function if verbose,
 *                 if not verbose, those parameters won't be use (send 0,0)
 *                 see error_rayon_partic, error_vitesse_partic
 * Return values :
 *  - true  : arguments can be use to create a particle
 *  - false : arguments invalid for the creation fo a particle
 */
bool part_validParams(double radius, POINT center, VECTOR speed,
                      bool verbose, ERREUR_ORIG origin, int id) {
    bool valid = false;

    if (vector_norm(speed) > MAX_VITESSE) {
        if (verbose) {
            error_vitesse_partic(origin, id);
        }
    } else if (radius>RMAX || radius<RMIN) {
        if (verbose) {
            error_rayon_partic(origin, id);
        }
    } else {
        valid = true;
    }

    return valid;
}


// ====================================================================
// String and file interface
/** Create particle from data in a string
 *      see part_create
 * Parameters :
 *  - string : to parse data from
 *             Expected format (all in double): radius posx posy vx vy
 * Return values :
 *  - if particle was successfully created
 *  - if false, error in string format or parameters value (see part_create)
 *              and appropriate errors will be displayed in terminal
 */
bool part_readData(const char *string) {
    double param[5] = {0};
    bool   success  = false;

    if (sscanf(string, "%lf %lf %lf %lf %lf", &param[0], &param[1], &param[2],
                                              &param[3], &param[4])==5) {

        if (part_create(param[0], point_create(param[1], param[2]),
            vector_create(param[3], param[4])) != UNASSIGNED) {
            success = true;
        }

    } else {
        error_lecture_elements(ERR_PARTIC, ERR_PAS_ASSEZ);
    }

    return success;
}

/** Append all the particles to a file
 *      write in the same format as string format in part_readData
 * Parameters : 
 *  - file : file to append to
 */
void part_saveAllData(FILE *file) {
    PARTICULE *part = NULL;

    if (list_goToFirst(&particles) != NULL) {
        do {
            part = list_getData(particles, LIST_CURRENT);
            fprintf(file, "%f %f %f %f %f\n" , part->radius,
                    part->center.x, part->center.y,
                    part->speed.x,  part->speed.y);
        } while (list_goToNext(&particles) != NULL);
    }
}


// ====================================================================
// Destructions
/** Delete particle with given ID
 * Parameters :
 *  - partID : id of particle to delete
 *             see part_create
 * Return values :
 *  - if particle was successfully deleted
 *  - if returns false probably already deleted of never existed
 */
bool part_deletePart(int partID) {

    if (list_getDataFromId(&particles, partID) == NULL) {
        return false;
    } else { // particule found in list and is the current one
        (void) list_deleteCurrent(&particles);
        return true;
    }


}

/** Delete all existing particles
 * Note : use with care
 */
void part_deleteAll() {
    list_deleteAll(&particles);
}


// ====================================================================
// Setters/Getters
/** Lock a given particle in place (or unlock)
 *      see PARTICULE structure
 * Parameters :
 *  - partID : id of particle to lock
 *             see part_create
 *  - lock : lock state to be applied to given particle
 * Return values :
 *  - if particle was successfully (un-)locked
 *  - if false, probably particle deleted of never existed
 */
bool part_setLock(int partID, bool lock) {
    PARTICULE *pPart = part_findPart(partID);

    if (pPart != NULL) {
        pPart->locked = lock;
        return true;
    } else {
        return false;
    }
}

// Append to pHead all particles' center
// Please, do not modify their values (except if you feel lucky)
void part_getAllCenters(LIST_HEAD *pHead) {
    PARTICULE *part = NULL;

    if (list_goToFirst(&particles) != NULL) {
        do {
            part = list_getData(particles, LIST_CURRENT);
            (void) list_add(pHead, &(part->center));
        } while (list_goToNext(&particles) != NULL);
    }
}

// return the total number of current particles
int part_totalNB() {
    return list_getNbElements(particles);
}


// ====================================================================
// Rendu specific functions
/* prints force norm between the two first particles */
void particule_force_rendu1() {
	double force_norm = 0;
    double distance = 0;

    PARTICULE *p1 = list_getData(particles, 1);
    PARTICULE *p2 = list_getData(particles, 2);

    if (p1!=NULL && p2!=NULL) {
        distance = point_distance(p1->center, p2->center);
        force_norm = part_calcForce(p1, p2, distance);
        printf("%8.3f\n", force_norm);

    } else {
        error_msg("Not enough particles for Force mode (need at least 2)");
    }
}

/* prints status of first particle after one tick of simulation */
void particule_integration_rendu2() {
    PARTICULE *part = NULL;

    part_nextTick(DELTA_T);

    // get the first particule created (id = 0)
    part = list_getDataFromId(&particles, 0);

    if (part!=NULL) {
        printf("%8.3f %8.3f\n", part->force.x, part->force.y);
        printf("%7.3f %7.3f %9.4f %9.4f\n", part->speed.x,  part->speed.y,
                                            part->center.x, part->center.y);
    } else {
        error_msg("No particule found. There must be at leat one particule\n");
    }
}


// ====================================================================
// Manage simulation
/** return ID of closest part to a point, which it overlaps
 * Parameters :
 *  - point : point to consider
 * Return values :
 *  - id of closest particles to the point and overlaping it
 *  - UNNASIGNED (=-1) if no particle overlap the point
 */
// TODO using sort
int part_closestPartOn(POINT point) {
    int partID = UNASSIGNED;
    double dist = RMAX+1; // init a maximum dist
    double newDist = 0;
    PARTICULE* current = NULL;

    if (list_goToFirst(&particles) != NULL) {
        do {
            current = list_getData(particles, LIST_CURRENT);
            newDist = point_distance(current->center, point);

            if (newDist < dist && newDist<current->radius) {
                dist = newDist;
                partID = current->id;
            }
        } while (list_goToNext(&particles) != NULL);
    }

    return partID;
}


// ====================================================================
// Drawings
// Diplay all particles
void part_display(void) {
    list_fctToAllElements(particles, part_draw);
}

// Draws a single particle
static void part_draw(void *pData) {
    PARTICULE *part = pData;

    if (part != NULL) {
        graphic_set_line_width(2.);
        part_setColor(part->speed);
        graphic_draw_circle(part->center, part->radius, CONTINUOUS);
    }
}

// set color of graphics given vector speed
static void part_setColor(VECTOR speed) {
	
	float red   = linear_interpolation(vector_norm(speed), 0, 0, 
									   MAX_VITESSE, 1  );
	float green = linear_interpolation(vector_norm(speed), 0, 0, 
									   MAX_VITESSE, 0.2);
	float blue  = linear_interpolation(vector_norm(speed), 0, 0, 
									   MAX_VITESSE, 0.2);
	
	graphic_set_color_3f(red, green, blue);
}


// ====================================================================
// Simulation update fct
/** Update kinematic of all particles
 * Parameters :
 *  - delta_t : time ammount to run the simulation on
 *              the smaller, the more precise the results
 *              valide domain : >=0
 */
void part_nextTick(double delta_t) {
    PARTICULE *part = NULL;

    if (delta_t>=0.) {
        //TODO part_collisionBlackHole();
        part_updateAllForces();

        // update kinematic for every particles
        if (list_goToFirst(&particles) != NULL) {
            do {
                part = list_getData(particles, LIST_CURRENT);
                updateKinematic(part, delta_t);
            } while (list_goToNext(&particles));
        }
    }
}

// update force value of all particles
static void part_updateAllForces() {

    list_fctToAllElements     (particles, initForceVec);
    list_fctToAll2combinations(particles, part_interact);
}

// set force vector to null
static void initForceVec(void *data) {
    if (data != NULL) {
        ((PARTICULE*)data)->force = vector_null();
    }
}

// apply interaction between 2 particlse
static void part_interact(void *dataA, void *dataB) {
    double distance = 0;
    double force_norm = 0;
    PARTICULE *a = dataA;
    PARTICULE *b = dataB;

    if (a!=NULL && b!=NULL) {
        distance = point_distance(a->center, b->center);

        force_norm = part_calcForce(a, b, distance);

        part_applyForce(a, b, distance, force_norm);
    }
}

// return force norm between 2 particles
static double part_calcForce(PARTICULE *p1, PARTICULE *p2, double distance) {
    double force_norm = 0;
    double seuil_d = p1->radius + p2->radius + fmin(p1->radius, p2->radius);

    if (distance < 3*seuil_d) {
        if (distance < EPSILON_ZERO) {
            force_norm = MAX_REP;
        } else if (distance < seuil_d) {
            force_norm = linear_interpolation(distance, 0, MAX_REP,
                                                        seuil_d, 0);
        } else if (distance < 2*seuil_d) {
            force_norm = linear_interpolation(distance, seuil_d, 0,
                                                        2*seuil_d, MAX_ATTR);
        } else { // distance < 3*seuil_d
            force_norm = linear_interpolation(distance, 2*seuil_d, MAX_ATTR,
                                                        3*seuil_d, 0);
        }
    }

    return force_norm;
}

// aplly the force norm to 2 particles
static void part_applyForce(PARTICULE *p1, PARTICULE *p2, double distance,
                                                          double force_norm) {
    VECTOR force = vector_null();

    if (distance < EPSILON_ZERO) {
        force = vector_create(0, force_norm);
    } else { // general case
        force = vector_create(force_norm/distance *
                             (p2->center.x-p1->center.x), 
                             (force_norm/distance) * 
                             (p2->center.y-p1->center.y));
    }

    p1->force = vector_sum(p1->force, force);
    p2->force = vector_sum(p2->force, vector_multiply(force, -1));
}

// update kinematic of given particle on interval delta_t
static void updateKinematic(PARTICULE* part, double delta_t) {
    double speed_norm = 0;

    if (part != NULL) {
        // update acceleration
        part->acceleration = vector_multiply(part->force, 1/part->mass);

        if (!part->locked) { // a locked particule doesn't build up speed
            // update speed
            part->speed = vector_sum(part->speed,
                                     vector_multiply(part->acceleration, delta_t));
            speed_norm  = vector_norm(part->speed);
    
            if (speed_norm > MAX_VITESSE) { // scale down the vector
                part->speed = vector_multiply(part->speed,
                                              MAX_VITESSE/speed_norm);
            }
    
            // update position
            part->center = point_translate(part->center,
                                           vector_multiply(part->speed, delta_t));
        }
    }
}


// ====================================================================
// set the mass of a particle (proportional to radius^2)
static void part_initMass(PARTICULE *part) {
    part->mass = pow(part->radius, 2) * KMASSE;
}

// ====================================================================
// utilities functions (managing datas tructure)
// return pointer to an empty slot in the data structure
static PARTICULE* newPart() {
    PARTICULE* newPart = malloc(sizeof(PARTICULE));

    if (!partList_initialized) {
        particles = list_create(deletePart, sortPart, idPart);
        partList_initialized = true;
    }

    if (newPart==NULL) {
        error_msg("Allocation failure in Particule module\n");
    }

    (void) list_add(&particles, newPart);

    return newPart;
}

// return NULL if part not found
static PARTICULE* part_findPart(int partID) {
    return (PARTICULE*) list_getDataFromId(&particles, partID);
}

static void deletePart(void *toDelete) {
    PARTICULE *part = (PARTICULE*) toDelete;

    if (part != NULL) {
        #if DEBUG
        printf("Freing particle no %d (locked = %d): %f %f %f %f %f\n", 
                part->id, part->locked, part->radius,
                part->center.x, part->center.y,
                part->speed.x, part->speed.y);
        #endif

        free(part);
    }
}

// Comparator of particles, from their x coordinate
// return -1 if a<b, 1 if a>b, 0 if a==b
static int sortPart(void *p_a, void *p_b) {
    PARTICULE *a = (PARTICULE*) p_a;
    PARTICULE *b = (PARTICULE*) p_b;

    if (a!=NULL && b!=NULL) {
        if (a->center.x > b->center.x) {
            return 1;
        } else if (a->center.x < b->center.x) {
            return -1;
        } else {
            ; // equality, probably won't happen
        }
    }

    return 0;
}

// return ID of particle
static int idPart(void *p_a) {
    int val = UNASSIGNED; 

    if (p_a != NULL) {
        val = ((PARTICULE*)p_a)->id;
    }

    return val;
}
