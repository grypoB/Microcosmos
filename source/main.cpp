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

extern "C"
{
#include "sim.h"
#include "constantes.h"
}


#define DELETE_KEY 'd'

// id for all callbacks
// file_cb
#define SAVE 0
#define LOAD 1
// simulation_cb
#define START 0
#define STEP  1


// Calulate  next simulations status
static void next_step(void);

// OpenGl callbacks
static void display(void);
static void reshape(int w, int h);
static void file_cb(int id);
static void simulation_cb(int id);
static void idle(void);
static void mouse(int button, int state, int x, int y);
static void keyboard(unsigned char key, int x, int y);

// init graphic display
static void initOpenGl(void);
static void initGlui(char* filename);

// Return the mode read from a string (argv[1])
// return MODE_UNSET if string wasn't valid
static MODE read_mode(const char string[]);


namespace
{
    // OpenGl window
    int main_window;

    // button controlling the simulation (start/stop)
    GLUI_Button *startButton;

    // field containing the names of file to save/open
    GLUI_EditText *saveFile;
    GLUI_EditText *loadFile;
	
    // field displaying the number of entities
    GLUI_EditText *nb_trou_noir;
    GLUI_EditText *nb_generateur;
    GLUI_EditText *nb_particule;

    bool simulation_running = false;

    // limits of displayed area (for glOrtho)
    GLfloat left = -RMAX, right = RMAX, 
            down = -RMAX, up    = RMAX;
}


int main (int argc, char *argv[])
{
    MODE mode = MODE_UNSET;  

    if(argc==3) {
        mode = read_mode(argv[1]);
    }
    else if(argc==1) {
		mode = DEFAULT;
	}

    switch(mode) {
        case ERROR: sim_openFile(argv[2], ERROR);
        break;
        case FORCE: sim_openFile(argv[2], FORCE);
        break;
        case INTEGRATION: sim_openFile(argv[2], INTEGRATION);
        break;
        case GRAPHIC: 
            sim_openFile(argv[2], GRAPHIC);
            glutInit(&argc, argv);
            initOpenGl();
            initGlui(argv[2]);
            glutMainLoop();
        break;
        case SIMULATION: 
            sim_openFile(argv[2], SIMULATION);
            // no break because same command afterwards for sim mode
        case DEFAULT :
            glutInit(&argc, argv);
            initOpenGl();
            initGlui(NULL);
            glutMainLoop();

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
    #ifdef DEBUG
    printf("Init Opengl...");
    #endif

    /* Initialise Glut and Create Window */
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    main_window = glutCreateWindow("Microcosmos");

    /* Fonctions callback */
    GLUI_Master.set_glutDisplayFunc(display);
    GLUI_Master.set_glutReshapeFunc(reshape);
    GLUI_Master.set_glutMouseFunc(mouse);
    GLUI_Master.set_glutKeyboardFunc(keyboard);
    GLUI_Master.set_glutIdleFunc(idle);

    #ifdef DEBUG
    printf("done\n");
    #endif
}

// send null if no file for the openfile field
static void initGlui(char* filename)
{

    GLUI *glui = NULL;
    // multiple panle used in the glui window
    GLUI_Panel *file        = NULL;
    GLUI_Panel *simulation  = NULL;
    GLUI_Panel *information = NULL;

    #ifdef DEBUG
    printf("Init GLUI interface ...");
    #endif

    /*Code GLUI pour l'interface*/
    glui = GLUI_Master.create_glui("GLUI");

    //File panel
    file     = glui->add_panel("File");
    loadFile = glui-> add_edittext_to_panel(file, "Filename",
											GLUI_EDITTEXT_TEXT);
    if (filename != NULL) {
        loadFile->set_text(filename);
    }
    glui-> add_button_to_panel(file,"Load", LOAD, file_cb);
    saveFile = glui-> add_edittext_to_panel(file, "Filename", 
											GLUI_EDITTEXT_TEXT);
	saveFile->set_text("save.txt");
    glui-> add_button_to_panel(file,"Save", SAVE, file_cb);


    //Simulation panel
    simulation = glui->add_panel("Simulation");
    startButton = glui->add_button_to_panel(simulation, "Start",
											START, simulation_cb);
    glui->add_button_to_panel(simulation, "Step",  STEP, simulation_cb);

    //Information panel
    information   = glui->add_panel("Information");
    nb_particule  = glui-> add_edittext_to_panel(information, "Nb Particule",
												 GLUI_EDITTEXT_INT); 
    nb_generateur = glui-> add_edittext_to_panel(information, "Nb Generateur",
												 GLUI_EDITTEXT_INT);
    nb_trou_noir  = glui-> add_edittext_to_panel(information, "Nb Trou noir",
												 GLUI_EDITTEXT_INT);

    glui->add_button( "Quit", EXIT_SUCCESS, (GLUI_Update_CB) exit);

    glui->set_main_gfx_window( main_window );

    #ifdef DEBUG
    printf("done\n");
    #endif
}

static void update_nbEntities() {
    int nbEntities[ENTITY_NB] = {0};
    
    sim_nbEntities(nbEntities);

    nb_generateur->set_int_val(nbEntities[GEN_SLOT]);
    nb_trou_noir ->set_int_val(nbEntities[BCKH_SLOT]);
    nb_particule ->set_int_val(nbEntities[PART_SLOT]);
}

static void idle()
{
    if ( glutGetWindow() != main_window ) 
        glutSetWindow(main_window);  

    if (simulation_running) {
        next_step();
    }

}

static void next_step(void)
{
    //met a jour simulation
    sim_next_step();
    
    //met a jour affichage
    glutPostRedisplay();
}

//Réponse à l'interface utilisateur partie simulation
static void simulation_cb(int id)
{
	switch(id)
    {
        case START:
            if (simulation_running) {
					startButton->set_name("Start");
					glutIdleFunc(NULL);
                simulation_running = false;
            } else {
					startButton->set_name("Stop");
					glutIdleFunc(idle);
                simulation_running = true;
            }
        break;

        case STEP:
            next_step();
        break;
            exit(0);
            break;

        default:
            printf("Wrong ID in %s\n", __func__);
    }
    glutPostRedisplay();
}


static void display(void)
{
    // reset the display
    glClearColor ( 1., 1., 1., 0. ); // background color 
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
	
    // set region to see
    glOrtho(left, right, down, up, -1.0, 1.0);

    // print all simulation entities
    sim_display();

    //met à jour nb éléments
    update_nbEntities();

    glutSwapBuffers();
}

static void reshape(int w, int h)
{
    double xmin, xmax, ymin, ymax;
    double base = 0, shift = 0;

    glViewport(0, 0, w, h);
	
    sim_extremPoints(&xmin, &xmax, &ymin, &ymax);
    
    left  = xmin - RMAX;
    right = xmax + RMAX;
    down  = ymin - RMAX;
    up    = ymax + RMAX;
    
    // update dimension for future glOrtho
    if ( (double)w/h > (right-left)/(up-down) ) {
        base   = (double) w*(up-down)/h;
        shift  = (base - (right-left))/2;
        left  -= shift;
        right += shift;
    } else {
        base  = (double) h*(right-left)/w;
        shift = (base - (up-down))/2;
        down -= shift;
        up   += shift;
    }

    #ifdef DEBUG
    printf("reshape : left=%f, right=%f, down=%f, up=%f\n",
                      left,    right,    down,    up);
    #endif

    glutPostRedisplay(); 
}

//Réponse à l'interface utilisateur partie fichier
static void file_cb(int id) {
    switch(id) {
        case SAVE:
			glutIdleFunc(NULL);
            sim_save(saveFile->get_text());
        break;
        case LOAD:
			glutIdleFunc(NULL);
            sim_clean();
            sim_openFile(loadFile->get_text(), SIMULATION);
            simulation_running = false;

            reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        break;
        default:
            printf("Wrong id in %s\n",__func__);
    }

    glutPostRedisplay(); 
}

static void mouse(int button, int state, int x, int y) {
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    #ifdef DEBUG
    printf("w=%d, h=%d\n",w,h);
    #endif

    if (button == GLUT_LEFT_BUTTON) {
        switch (state) {
            case GLUT_DOWN:
                sim_select((double) x/w*(right-left)+left, 
                           (double) -y/h*(up-down)+up);
            break;
            case GLUT_UP:
                sim_deselect();
            break;
        }
    }
}

static void keyboard(unsigned char key, int x, int y) {
    if (key == DELETE_KEY) {
        sim_deleteSelection();
    }
}
