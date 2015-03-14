#include <GL/glu.h>
#include <GL/glui.h>

#include "constantes.h"

namespace
{
	GLfloat aspect_ratio ;
	int main_window;
	GLUI_Panel *file;
	GLUI_Panel *simulation;
	GLUI_Panel *information;
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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
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
	glui-> add_edittext_to_panel(file, "Filename", GLUI_EDITTEXT_TEXT, NULL, -1, control_cb);
    glui-> add_button_to_panel(file,"Load", -1, control_cb);
    
    glui-> add_edittext_to_panel(file, "Filename", GLUI_EDITTEXT_TEXT, NULL, -1, control_cb);
    glui-> add_button_to_panel(file,"Save", -1, control_cb);
    
	
	//Simulation avec start et step
	simulation = glui->add_panel("Simulation" );
	glui->add_button_to_panel( simulation ,"Start");  //bouton pour commencer/stopper la simulation
	glui->add_button_to_panel( simulation ,"Step");   //lorsque la simulation est stoppée -> calcule seulement un pas

	
	//Information avec nb elements
	information = glui->add_panel("Information" );
	glui-> add_edittext_to_panel(information, "Nb Particule", GLUI_EDITTEXT_TEXT, NULL, -1, control_cb);
	glui-> add_edittext_to_panel(information, "Nb Generateur", GLUI_EDITTEXT_TEXT, NULL, -1, control_cb);
	glui-> add_edittext_to_panel(information, "Nb Trou noir", GLUI_EDITTEXT_TEXT, NULL, -1, control_cb);
	
	glui->add_button( "Quit",0,(GLUI_Update_CB)exit );  //doit fermer les fenetres et et fichiers et quitter le programme
	
	/*quand un widget de GLUI change, un évènement de redisplay est envoyé à la main_gfx_window */
	glui->set_main_gfx_window( main_window );
	
	/**** Regular GLUT main loop ****/
	glutMainLoop();

}
