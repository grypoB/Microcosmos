/* Nom: main.cpp
 * Description: module sous-système de contrôle: gestion du dialogue 
				avec l'utilisateur et interface graphique
 * Date: 08.02.2014
 * version : 1.0
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glui.h>
//#include <GL/glut.h>
#include <string.h>

extern "C"
{
#include "constantes.h"
#include "sim.h"
}

namespace
{
	GLUI* Glui;
	GLfloat aspect_ratio ;
	int main_window;
	GLUI_Panel *file;
	GLUI_Panel *simulation;
	GLUI_Panel *information;
	char* live_var;
}

/* Variables to count the elapsed time
int minutes = 0;
int seconds = 0;
int milliseconds = 0;
*/

enum Boutton {START, STEP, QUIT};
int elementnb[ENTITY_NB];

//fonction pour GLUI
void display(void);
void reshape(int w, int h);
void load_cb(int live_var);
void save_cb(int live_var);
void simulation_cb(int control);
void keyboard(unsigned char Key, int x, int y);
void mouse(int button, int button_state, int x, int y );
void move_entity(int x, int y);
void idle(void);

//fonction pour le mode GRAPHIC
void initOpenGl(void);

//void timer_cb(int value);

// Mode of the simulation to be ran on, see specs sheet for details
enum Mode {ERROR, FORCE, INTEGRATION, GRAPHIC, SIMULATION, MODE_UNSET};
typedef enum Mode MODE;

// Return the mode read from a string (argv[1])
// return MODE_UNSET if string wasn't valid
static MODE read_mode(const char string[]);


int main (int argc, char *argv[])
{ 
	enum Mode mode;
    mode = MODE_UNSET;  
    
	if(argc==3)
	{
		mode = read_mode(argv[1]);
	}
	switch(mode) 
	{
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
		break;
		case SIMULATION: //sim_simulation(argv[2]);
		break;
		case MODE_UNSET: printf("Syntaxe attendue : sim.x [Error"
								"|Force|Integration|Graphic|Simulation,"
								"nom_fichier]\n");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

static MODE read_mode(const char string[])
{
	MODE mode;
	if(strcmp(string, "Error" ) == 0) 
    { 
		mode = ERROR;
    }
    else if (strcmp(string, "Force" ) == 0)
    { 
		mode = FORCE;
    }
    else if (strcmp(string, "Integration" ) == 0)
    { 
		mode = INTEGRATION;
    }
    else if (strcmp(string, "Graphic" ) == 0)
    { 
		mode = GRAPHIC;
    }
    else if (strcmp(string, "Simulation" ) == 0)
    { 
		mode = SIMULATION;
    }
    else 
    { 
		mode = MODE_UNSET;
    }
    return mode;
}

void initOpenGl()
{
	/*Initialise Glut and Create Window*/
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(250, 250);
	
	main_window = glutCreateWindow("Microcosmos");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	
	/* Fonctions callback*/
	GLUI_Master.set_glutDisplayFunc(display);
	GLUI_Master.set_glutReshapeFunc(reshape);
	GLUI_Master.set_glutMouseFunc(mouse);
	glutMotionFunc(move_entity);
	GLUI_Master.set_glutKeyboardFunc(keyboard);
	GLUI_Master.set_glutIdleFunc(idle);

	//glutTimerFunc(100, &timer_cb, 0);
		
	/*Code GLUI pour l'interface*/
	GLUI *glui = GLUI_Master.create_glui( "GLUI", 0, 400, 50 );
	
	//File
	file = glui->add_panel("File" );
	glui-> add_edittext_to_panel(file, "Filename", GLUI_EDITTEXT_TEXT, live_var, -1, load_cb);
	glui-> add_button_to_panel(file,"Load", -1, load_cb);
	glui-> add_edittext_to_panel(file, "Filename", GLUI_EDITTEXT_TEXT, live_var, -1, save_cb);
	glui-> add_button_to_panel(file,"Save", -1, save_cb);            					//A FINIR
    
	//Simulation
	simulation = glui->add_panel("Simulation");
	glui->add_button_to_panel(simulation ,"Start", START, simulation_cb);
	glui->add_button_to_panel(simulation ,"Step", STEP, simulation_cb);					//A FINIR
	
	//Information
	information = glui->add_panel("Information" );
	GLUI_EditText *nb_particule = glui-> add_edittext_to_panel(information, "Nb Particule", GLUI_EDITTEXT_INT); 
	nb_particule->set_int_val(elementnb[PART_SLOT]);  
	GLUI_EditText *nb_generateur = glui-> add_edittext_to_panel(information, "Nb Generateur", GLUI_EDITTEXT_INT);
	nb_generateur->set_int_val(elementnb[GEN_SLOT]);
	GLUI_EditText *nb_trou_noir = glui-> add_edittext_to_panel(information, "Nb Trou noir", GLUI_EDITTEXT_INT);
	nb_trou_noir->set_int_val(elementnb[BCKH_SLOT]);
	
	glui->add_button( "Quit", QUIT, (GLUI_Update_CB) simulation_cb);
	
	glui->set_main_gfx_window( main_window );
	glutMainLoop();
}

void next_step(void)
{
	//mettre a jour information
	sim_nbEntities(elementnb);
	
	//met a jour simulation
	
	//met a jour affichage
	display();
}

void idle()                                       
{
	enum State {ON, OFF};
	enum State state;
	state = ON;
	
	if ( glutGetWindow() != main_window ) 
		glutSetWindow(main_window);  
	
	switch(state)
	{
		case ON : next_step();
		break;
		case OFF :
		break;
		default :
		break;
	}
  glutPostRedisplay();
}

void simulation_cb(int control)
{
	enum State {ON, OFF};
	enum State state;
	state = ON;
		
	switch(control)    
	{
		case START:
				if(state==ON)
				{
					glutIdleFunc(NULL);
					state = OFF;
				}
				else if(state==OFF)
				{
					glutIdleFunc(idle);
					state = ON;
				}			
				glutSetWindow(main_window);
				glutPostRedisplay();
				break;
				
		case STEP:
				next_step();
				glutIdleFunc(NULL);
				state = OFF;
				glutSetWindow(main_window);
				glutPostRedisplay();
				break;

		case QUIT:
			Glui->close();						
			glutSetWindow(main_window);
			glFinish();
			glutDestroyWindow(main_window);
			exit(0);
			break;

		default:
			fprintf(stderr, "Don't know what to do with Button ID %d\n", control);
	}
}

	
void display(void)
{
	//sim_display appelle chaque module qui gerent elles memes leur affichage
	sim_display();
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	//appel sim pour savoir centre de masse
	sim_centre_masse();                              //devrait renvoyer un point et non un double
	
	GLfloat left = (sim_centre_masse()) - RMAX, 
			right= (sim_centre_masse()) + RMAX, 
			down = (sim_centre_masse()) - RMAX, 
			up   = (sim_centre_masse()) + RMAX;
	
	if (aspect_ratio <= 1.)
	glOrtho(left, right, down/aspect_ratio, up/aspect_ratio, -1.0, 1.0);
	
	else
	glOrtho(left*aspect_ratio, right*aspect_ratio, down, up, -1.0, 1.0);
	
	
	//rapport entre les dimensions X/Y du domaine Open GL et taille en pixels du widget glut //exo8 serie19
	
	glViewport(0, 0, w, h);
	aspect_ratio = (GLfloat) w / (GLfloat) h ;
	glutPostRedisplay(); 
}

void load_cb(int control, const char* live_var)
{
	sim_graphic(live_var);
	glutIdleFunc(NULL);
}

void save_cb(int control, const char* live_var)
{
	//glutIdleFunc(NULL);
	//enregistre etat actuel de simulation dans ce fichier
	FILE *file = NULL;
	file = fopen(live_var,"w");
	
	fprintf(file, "%s", sim_write());

	fclose(file);
}

void mouse(int button, int button_state, int x, int y )
{
  while (button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN ) 
  {
    //particule la plus proche selectionnée                   //part_closestPart(POINT point);
    //doit rester immobile
    //peut aussi selectionner generateur ou trou_noir
    
    //set_point_cb                    //coordonnées x et y de la souris (differentes coordonnes a convertir pour opengl: exo3 serie19)
  }
}

void move_entity(int x, int y)
{
	//deplacer entitée séléctionnée
}

void keyboard(unsigned char Key, int x, int y)
{
	if(1)  // si une entitée est selectionnée
	{	switch(Key)
		{
			case 'd': //détruire entitée  
			break;
			default : //rien
			break;
		}
	}
  
  glutPostRedisplay();
}

/*void timerCB(int value)                      //fonction du prof (pas sure si besoin pour STEP)
{
	// Set the timer to 100 milliseconds.
	// It means that it will be called 10 times a second.
	glutTimerFunc(100, &timerCB, 0);
	
	// Update the milliseconds count
	++milliseconds;
	
	// If elapsed milliseconds reach to 10 x 100 = 1000
	// reset it and increment seconds.
	if(milliseconds == 10)
	{
		milliseconds = 0;
		++seconds;
		
		// If elapsed seconds reach to 60
		// reset it and increment minutes
		if(seconds == 60)
		{
		seconds = 0;
		++minutes;
		}
	}
	glutPostRedisplay();
}*/


