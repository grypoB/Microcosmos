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
}

//fonction pour opengl
void affichage(void);
void reshape(int w, int h);

//fonction pour le mode GRAPHIC
void initOpenGl(void);

//fonction callback (interface)
void file_cb(int control);
void control_cb(int control);

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
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(250, 250);
	
	main_window = glutCreateWindow("Microcosmos");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	
	glutDisplayFunc(affichage);
	glutReshapeFunc(reshape);
	
	GLUI *glui = GLUI_Master.create_glui( "GLUI", 0, 400, 50 );
	
	//File avec filename load et save
	file = glui->add_panel("File" );
	glui-> add_edittext_to_panel(file, "Filename", GLUI_EDITTEXT_TEXT, NULL, -1, file_cb);
    glui-> add_button_to_panel(file,"Load", -1, file_cb);
    
    glui-> add_edittext_to_panel(file, "Filename", GLUI_EDITTEXT_TEXT, NULL, -1, file_cb);
    glui-> add_button_to_panel(file,"Save", -1, file_cb);
    
	
	//Simulation avec start et step
	simulation = glui->add_panel("Simulation" );
	glui->add_button_to_panel( simulation ,"Start", -1, GLUI_CB());  //bouton pour commencer/stopper la simulation
	glui->add_button_to_panel( simulation ,"Step", -1, GLUI_CB());   //lorsque la simulation est stoppée -> calcule seulement un pas

	
	//Information avec nb elements
	information = glui->add_panel("Information" );
	glui-> add_edittext_to_panel(information, "Nb Particule", GLUI_EDITTEXT_INT, NULL, -1, control_cb);
	glui-> add_edittext_to_panel(information, "Nb Generateur", GLUI_EDITTEXT_INT, NULL, -1, control_cb);
	glui-> add_edittext_to_panel(information, "Nb Trou noir", GLUI_EDITTEXT_INT, NULL, -1, control_cb);
	
	glui->add_button( "Quit",0,(GLUI_Update_CB)exit );  //doit encore fermer les fenetres et fichiers 
	
	/*quand un widget de GLUI change, un évènement de redisplay est envoyé à la main_gfx_window */
	glui->set_main_gfx_window( main_window );
	
	/**** Regular GLUT main loop ****/
	glutMainLoop();

}

void file_cb(int control)
{
	
}

void control_cb(int control)
{
	
}


void affichage(void)
{
	// variables qui définissent le domaine visualisé avec Ortho
	GLfloat gauche= -RMAX, droite = RMAX, bas= -RMAX, haut= RMAX;
	glClear(GL_COLOR_BUFFER_BIT);
	
	// ré-initialisation de la matrice de transformation
	glLoadIdentity();
	
	if (aspect_ratio <= 1.)
	glOrtho(gauche, droite, bas/aspect_ratio, haut/aspect_ratio, -1.0, 1.0);
	
	else
	glOrtho(gauche*aspect_ratio, droite*aspect_ratio, bas, haut, -1.0, 1.0);
	
	//fct qui créent particules
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	//le viewport prend toute la fenêtre
	glViewport(0, 0, w, h);
	
	// mise à jour pour usage futur par affichage()
	aspect_ratio = (GLfloat) w / (GLfloat) h ;
	
	// création d'un event demandant un ré-affichage
	glutPostRedisplay();
}
