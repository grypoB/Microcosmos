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
#include <GL/glu.h>
#include <GL/glui.h>

#define NB_PARTICULE 1
#define NB_GENERATEUR 2
#define NB_TROU_NOIR 3

extern "C"
{
#include "constantes.h"
#include "sim.h"
}

namespace
{
	GLfloat aspect_ratio ;
	int main_window;
	GLUI_Panel *file;
	GLUI_Panel *simulation;
	GLUI_Panel *information;
	char* live_var;
}

//fonction pour opengl
void affichage(void);
void reshape(int w, int h);
void keyboard(unsigned char Key, int x, int y);
void mouse(int button, int button_state, int x, int y );
void idle(void);


//fonction pour le mode GRAPHIC
void initOpenGl(void);

//fonction callback (interface)
void load_cb(int control);
void save_cb(int control);
void simulation_cb(int control);
void information_cb(int control);
void quit_cb(int control);

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
		case GRAPHIC: //sim_graphic(argv[2]);
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

void initOpenGl(void)
{
	/*Initialise Glut and Create Window*/
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // ??
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(250, 250);
	
	main_window = glutCreateWindow("Microcosmos");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	
	glutDisplayFunc(affichage);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	
	/*
	 * AJOUTER OPENGL appeler graphic.c ici
	 * revoir aussi affichage
	 */
		
	/*Code GLUI pour l'interface*/
	GLUI *glui = GLUI_Master.create_glui( "GLUI", 0, 400, 50 );
	
	file = glui->add_panel("File" );
	glui-> add_edittext_to_panel(file, "Filename", GLUI_EDITTEXT_TEXT, live_var, -1, load_cb);
    glui-> add_button_to_panel(file,"Load", -1, load_cb);
    
    glui-> add_edittext_to_panel(file, "Filename", GLUI_EDITTEXT_TEXT, live_var, -1, save_cb);
    glui-> add_button_to_panel(file,"Save", -1, save_cb);
    
	
	simulation = glui->add_panel("Simulation" );
	glui->add_button_to_panel( simulation ,"Start", 1, simulation_cb);
	glui->add_button_to_panel( simulation ,"Step", 2, simulation_cb);
	
	information = glui->add_panel("Information" );
	glui-> add_edittext_to_panel(information, "Nb Particule", GLUI_EDITTEXT_INT, NULL, NB_PARTICULE, information_cb); 
	glui-> add_edittext_to_panel(information, "Nb Generateur", GLUI_EDITTEXT_INT, NULL, NB_GENERATEUR, information_cb);
	glui-> add_edittext_to_panel(information, "Nb Trou noir", GLUI_EDITTEXT_INT, NULL, NB_TROU_NOIR, information_cb);
	
	glui->add_button( "Quit",0, quit_cb );
	
	glui->set_main_gfx_window( main_window );
	
	GLUI_Master.set_glutIdleFunc(idle);

	glutMainLoop();

}

void load_cb(int control, char live_var)
{
	// recoit un nom pour openfile
	//ouvre le fichier dont le nom est donné
}

void save_cb(int control)
{
	//recoit un nom pour enregistrer
	//enregistre etat actuel de simulation dans ce fichier
}

void simulation_cb(int control)
{
	//start:bouton pour commencer/stopper la simulation
	//step:lorsque la simulation est stoppée -> calcule seulement un pas // enable seulement quand simulation est stoppée
}
void information_cb(int control)
{
	switch(control)
	{
		case NB_PARTICULE: //mettre a jour
		break;
		case NB_GENERATEUR: //mettre a jour
		break;
		case NB_TROU_NOIR: //mettre a jour
		break;
		
		return;
	}
}

void quit_cb(int control)
{
	//fermer les fenetres et fichiers puis quitter programme
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
	
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	aspect_ratio = (GLfloat) w / (GLfloat) h ;
	glutPostRedisplay(); 
}

void mouse(int button, int button_state, int x, int y )
{
  if (button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN ) 
  {
    //particule la plus proche selectionnée 
    //doit rester immobile
    //peut aussi selectionner generateur ou trou_noir
  }
}

void keyboard(unsigned char Key, int x, int y)
{
	if(1)  // si une entitée est selectionnée
	{	switch(Key)
		{
			case 'd': //détruire entitée
			break;
			default :
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

