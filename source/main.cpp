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

// Variables to count the elapsed time
int minutes = 0;
int seconds = 0;
int milliseconds = 0;

enum Boutton {START, STEP, QUIT};

//fonction pour opengl
void affichage(void);
void reshape(int w, int h);
void keyboard_cb(unsigned char Key, int x, int y);
void mouse_cb(int button, int button_state, int x, int y );
void idle(void);

//fonction pour le mode GRAPHIC
void initOpenGl(int * nb_element);

void timer_cb(int value);

//fonction callback (interface)
void load_cb(int live_var);
void save_cb(int live_var);
void simulation_cb(int control);

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
    int* nb_element;   
    
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
			 nb_element = sim_graphic(argv[2]);
			 glutInit(&argc, argv);
			 initOpenGl(nb_element);
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

void initOpenGl(int * nb_element)
{
	/*Initialise Glut and Create Window*/
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // ??
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(250, 250);
	
	main_window = glutCreateWindow("Microcosmos");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	
	glutDisplayFunc(affichage);
	glutReshapeFunc(reshape);
	
	glutKeyboardFunc(keyboard_cb);
	glutMouseFunc(mouse_cb);

	// Variables to count the elapsed time
	
	glutTimerFunc(100, &timer_cb, 0);
		
	/*Code GLUI pour l'interface*/
	GLUI *glui = GLUI_Master.create_glui( "GLUI", 0, 400, 50 );
	
	//File
	file = glui->add_panel("File" );
	
	glui-> add_edittext_to_panel(file, "Filename", GLUI_EDITTEXT_TEXT, live_var, -1, load_cb);
	glui-> add_button_to_panel(file,"Load", -1, load_cb);

	glui-> add_edittext_to_panel(file, "Filename", GLUI_EDITTEXT_TEXT, live_var, -1, save_cb);
	glui-> add_button_to_panel(file,"Save", -1, save_cb);
    
	//Simulation
	simulation = glui->add_panel("Simulation" );
	glui->add_button_to_panel(simulation ,"Start", START, simulation_cb);
	glui->add_button_to_panel(simulation ,"Step", STEP, simulation_cb);					//A FINIR
	
	
	//Information
	information = glui->add_panel("Information" );
	GLUI_EditText *nb_particule = glui-> add_edittext_to_panel(information, "Nb Particule", GLUI_EDITTEXT_INT); 
	nb_particule->set_int_val(nb_element[0]); 
	
	GLUI_EditText *nb_generateur = glui-> add_edittext_to_panel(information, "Nb Generateur", GLUI_EDITTEXT_INT);
	nb_generateur->set_int_val(nb_element[1]);
	
	GLUI_EditText *nb_trou_noir = glui-> add_edittext_to_panel(information, "Nb Trou noir", GLUI_EDITTEXT_INT);
	nb_trou_noir->set_int_val(nb_element[2]);
	
	
	glui->add_button( "Quit", QUIT, (GLUI_Update_CB) simulation_cb);
	
	
	glui->set_main_gfx_window( main_window );
	
	//Callback
	GLUI_Master.set_glutMouseFunc( mouse_cb );
	GLUI_Master.set_glutKeyboardFunc( keyboard_cb );
	GLUI_Master.set_glutIdleFunc(idle);

	glutMainLoop();

}

void load_cb(int control, const char* live_var)
{
	sim_graphic(live_var);
	glutIdleFunc(NULL);
}

void save_cb(int control, const char* live_var)
{
	//enregistre etat actuel de simulation dans ce fichier
	FILE *fichier = NULL;
	fichier = fopen(live_var,"w");
	
	fprintf(fichier, "%s", sim_ecriture());

	fclose(fichier);
}

void simulation_cb(int control)
{
	enum Etat {ON, OFF};
	enum Etat etat;
	etat = ON;
		
	switch(control)    
	{
		case START:
				if(etat==ON)
				{
					glutIdleFunc(NULL);
					etat = OFF;
				}
				else if(etat==OFF)
				{
					glutIdleFunc(idle);
					etat = ON;
				}			
				glutSetWindow( main_window );
				glutPostRedisplay( );
				break;
				
		case STEP:
				glutIdleFunc(NULL);
				etat = OFF;
				//calcule seulement un pas
				glutSetWindow(main_window);
				glutPostRedisplay();
				break;

		case QUIT:
			Glui->close();						//ferme fenetre glui
			glutSetWindow(main_window);
			glFinish();							//ferme les images
			glutDestroyWindow(main_window);		//ferme fenetre image
			exit(0);							//quitte programme
			break;

		default:
			fprintf(stderr, "Don't know what to do with Button ID %d\n", control);
	}
	
}


void affichage(void)
{
	GLfloat gauche= -RMAX, droite = RMAX, bas= -RMAX, haut= RMAX;
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
	
	if (aspect_ratio <= 1.)
	glOrtho(gauche, droite, bas/aspect_ratio, haut/aspect_ratio, -1.0, 1.0);
	
	else
	glOrtho(gauche*aspect_ratio, droite*aspect_ratio, bas, haut, -1.0, 1.0);
	
	//voir: 6. Affichage et interaction dans la fenetre graphique
	//rapport entre les dimensions X/Y du domaine Open GL et taille en pixels du widget glut //exo8 serie19

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	aspect_ratio = (GLfloat) w / (GLfloat) h ;
	glutPostRedisplay(); 
}

void mouse_cb(int button, int button_state, int x, int y )
{
  while (button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN ) 
  {
    //particule la plus proche selectionnée                   //part_closestPart(POINT point);
    //doit rester immobile
    //peut aussi selectionner generateur ou trou_noir
    
    //set_point_cb                    //coordonnées x et y de la souris (differentes coordonnes a convertir pour opengl: exo3 serie19)

  }
}

void keyboard_cb(unsigned char Key, int x, int y)
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

void idle(void)                                       
{
	if ( glutGetWindow() != main_window ) 
		glutSetWindow(main_window);  

  glutPostRedisplay();
}

void timerCB(int value)                      //fonction du prof (pas sure si besoin pour STEP)
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
}




