/* Nom: main.cpp
 * Description: module sous-système de contrôle: gestion du dialogue 
 avec l'utilisateur et interface graphique
 * Date: 22.03.2014
 * version : 1.0
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glui.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

extern "C"
{
#include "sim.h"
#include "constantes.h"
}


// id for all callbacks
// file_cb
#define SAVE 0
#define LOAD 1
// simulation_cb
#define START 0
#define STEP  1


void next_step(void);

//fonction pour GLUI
void display(void);
void reshape(int w, int h);
void file_cb(int id);
void simulation_cb(int id);
void idle(void);

//fonction pour le mode GRAPHIC
static void initOpenGl(void);
static void initGlui();

// Mode of the simulation to be ran on, see specs sheet for details
typedef enum Mode {ERROR, FORCE, INTEGRATION, GRAPHIC, SIMULATION, MODE_UNSET} MODE;

// Return the mode read from a string (argv[1])
// return MODE_UNSET if string wasn't valid
static MODE read_mode(const char string[]);

namespace
{
    //GLUI* glui;
    int main_window;
    GLUI_Panel *file;
    GLUI_Panel *simulation;
    GLUI_Panel *information;

    GLUI_EditText *saveFile;
    GLUI_EditText *loadFile;

    GLUI_EditText *nb_trou_noir;
    GLUI_EditText *nb_generateur;
    GLUI_EditText *nb_particule;

    bool simulation_running = false;

    GLfloat left = -50, 
            right= 50, 
            down = -50, 
            up   = 50;
    double aspect_ratio;
}


int main (int argc, char *argv[])
{
    MODE mode = MODE_UNSET;  
    

    if(argc==3) {
        mode = read_mode(argv[1]);
    }

    switch(mode) {
        case ERROR: sim_error(argv[2]);
        break;
        case FORCE: sim_force(argv[2]);
        break;
        case INTEGRATION: //sim_integration(argv[2]);
        break;
        case GRAPHIC: 
            sim_graphic(argv[2]);
            glutInit(&argc, argv);
            initOpenGl();
            glutMainLoop();
        break;
        case SIMULATION: 
            sim_simulation(argv[2]);
        break;

        case MODE_UNSET:
        default :
            printf("Syntaxe attendue : sim.x "
                   "[Error|Force|Integration|Graphic|Simulation,"
                   "nom_fichier]\n");
    }

    return EXIT_SUCCESS;
}

static MODE read_mode(const char string[])
{
    MODE mode = MODE_UNSET;

    if        (strcmp(string, "Error" )       == 0) { 
        mode = ERROR;
    } else if (strcmp(string, "Force" )       == 0) { 
        mode = FORCE;
    } else if (strcmp(string, "Integration" ) == 0) { 
        mode = INTEGRATION;
    } else if (strcmp(string, "Graphic" )     == 0) { 
        mode = GRAPHIC;
    } else if (strcmp(string, "Simulation" )  == 0) { 
        mode = SIMULATION;
    } else { 
        mode = MODE_UNSET;
    }

    return mode;
}

static void initOpenGl()
{
    /*Initialise Glut and Create Window*/
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(250, 250);

    main_window = glutCreateWindow("Microcosmos");

    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    #ifdef DEBUG
    printf("initOpengl\n");
    #endif

    /* Fonctions callback*/
    GLUI_Master.set_glutDisplayFunc(display);
    GLUI_Master.set_glutReshapeFunc(reshape);
    //GLUI_Master.set_glutMouseFunc(mouse);
    //glutMotionFunc(move_entity);
    //GLUI_Master.set_glutKeyboardFunc(keyboard);
    GLUI_Master.set_glutIdleFunc(idle);

    initGlui();
}

static void initGlui() {
    /*Code GLUI pour l'interface*/
    GLUI *glui = GLUI_Master.create_glui( "GLUI", 0, 400, 50 );

    //File
    file     = glui->add_panel("File" );
    loadFile = glui-> add_edittext_to_panel(file, "Filename", 
											GLUI_EDITTEXT_TEXT);
    glui-> add_button_to_panel(file,"Load", LOAD, file_cb);
    saveFile = glui-> add_edittext_to_panel(file, "Filename", 
											GLUI_EDITTEXT_TEXT);
    glui-> add_button_to_panel(file,"Save", SAVE, file_cb);


    //Simulation
    simulation = glui->add_panel("Simulation");
    glui->add_button_to_panel(simulation ,"Start", START, simulation_cb);
    glui->add_button_to_panel(simulation ,"Step",  STEP,  simulation_cb);

    //Information
    information   = glui->add_panel("Information" );
    nb_particule  = glui-> add_edittext_to_panel(information, "Nb Particule", 
												 GLUI_EDITTEXT_INT); 
    nb_generateur = glui-> add_edittext_to_panel(information, "Nb Generateur", 
												 GLUI_EDITTEXT_INT);
    nb_trou_noir  = glui-> add_edittext_to_panel(information, "Nb Trou noir", 
												 GLUI_EDITTEXT_INT);

    glui->add_button( "Quit", EXIT_SUCCESS, (GLUI_Update_CB) exit);

    glui->set_main_gfx_window( main_window );
}

void update_nbEntities() {
    int nbEntities[ENTITY_NB] = {0};
    
    sim_nbEntities(nbEntities);

    nb_generateur->set_int_val(nbEntities[GEN_SLOT]);
    nb_trou_noir ->set_int_val(nbEntities[BCKH_SLOT]);
    nb_particule ->set_int_val(nbEntities[PART_SLOT]);
}

void idle()
{
    if ( glutGetWindow() != main_window ) 
        glutSetWindow(main_window);  

    if (simulation_running) {
        next_step();
    }

    glutPostRedisplay();
}

void next_step(void)
{
    //met a jour simulation
    sim_next_step();
    

    //met a jour affichage
    glutPostRedisplay();
}

//Réponse à l'interface utilisateur partie simulation
void simulation_cb(int id)
{
    switch(id)
    {
        case START:
            if (simulation_running) {
				glutIdleFunc(NULL);
                simulation_running = false;
            } else {
				glutIdleFunc(idle);
                simulation_running = true;
            }
            // TODO change name of button
            // via hiding/showing the button ?
        break;

        case STEP:
            next_step();
        break;
            exit(0);
            break;

        default:
            printf("Wrong ID in %s\n", __func__);
    }
}


void display(void)
{
    glClearColor ( 1., 1., 1., 0. );       // specifie la couleur 
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    // update glOrtho
    if (aspect_ratio <= 1.)
        glOrtho(left, right, down/aspect_ratio, up/aspect_ratio, -1.0, 1.0);

    else
        glOrtho(left*aspect_ratio, right*aspect_ratio, down, up, -1.0, 1.0);

    sim_display();

    //met à jour nb éléments
    update_nbEntities();


    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
	/*
    left  = *xmin - RMAX;
    right = *xmax + RMAX;
    down  = *ymin - RMAX;
    up    = *ymax + RMAX;
    */
    aspect_ratio = (float) w/h;

    glutPostRedisplay(); 
}


//Réponse à l'interface utilisateur partie fichier
void file_cb(int id) {
    switch(id) {
        case SAVE:
			glutIdleFunc(NULL);
            sim_save(saveFile->get_text());
        break;
        case LOAD:
			glutIdleFunc(NULL);
            sim_clean();
            sim_simulation(loadFile->get_text());
            simulation_running = false;

            reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        break;
        default:
            printf("Wrong id in %s\n",__func__);
    }

    glutPostRedisplay(); 
}
