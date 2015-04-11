/* Nom: graphic.c
 * Description: module pour les dessins Opengl
 * Date: 08.02.2014
 * version : 1.1
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#define NB_COTES 150

#define DASH_SIZE 2

#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "graphic.h"
#include "geometry.h"
#include "constantes.h"

#define SIZE 2

//draws a circle : dashed lines or continuous lines can be chosen
void graphic_draw_circle(POINT center, double radius, DETAIL detail)
{
	int i,j;
	
	switch(detail)
	{
		case CONTINUOUS :
					glBegin (GL_LINE_LOOP);
					
					for (i=0; i < NB_COTES; i++)
					{
						float alpha = i * 2. * M_PI / NB_COTES;
						float x = center.x + radius * cos (alpha);
						float y = center.y + radius * sin (alpha);
						glVertex2f(x,y);
					}
	                glEnd ();
					break;
					
		case DASH_LINE :
					
					for (i=0; i<NB_COTES; i++)
					{
                        if ((i/DASH_SIZE)%2 == 0) {
                            glBegin(GL_LINE_STRIP);
                            for (j=0; j<DASH_SIZE+1 ; j++) {
                                float alpha = (i+j) * 2. * M_PI / NB_COTES;
                                float x = center.x + radius * cos (alpha);
                                float y = center.y + radius * sin (alpha);
                                glVertex2f(x,y);
                            }
                            i+= j;
                            glEnd();
                        }
					}
					break;
		case DISC :
					glBegin (GL_POLYGON);
					
					for (i=0; i < NB_COTES; i++)
					{
						float alpha = i * 2. * M_PI / NB_COTES;
						float x = center.x + radius * cos (alpha);
						float y = center.y + radius * sin (alpha);
						glVertex2f(x,y);
					}
	                glEnd ();
					break;
					
		default : printf("Wrong details in %s\n",__func__);
					break;
	}

}

//draws a vector
void graphic_draw_vector(POINT center, VECTOR vector)
{
	glBegin (GL_LINES);
	  glVertex2f (center.x, center.y);
      glVertex2f (center.x + vector.x, center.y +vector.y);
	glEnd ();
	
	glBegin (GL_TRIANGLES);
	  glVertex2f(center.x + vector.x - 2, center.y + vector.y);
	  glVertex2f(center.x + vector.x, center.y + vector.y + 2);
	  glVertex2f(center.x + vector.x + 2, center.y + vector.y);
	glEnd ();

}

void graphic_draw_polygon(int nb_sides, POINT center)
{
	int i;
	float alpha = 0;
	
	glBegin (GL_POLYGON);
	
	for(i=0; i<nb_sides; i++)
	{
		  alpha = i * 2. * M_PI / nb_sides;
		  glVertex2f (center.x + SIZE * cos (alpha), 
					  center.y + SIZE * sin (alpha));
	}
	  
	glEnd ();
}

//sets the color of the element
void graphic_set_color(GRAPHIC_COLOR color) 
{
    switch (color) 
    {
        case AQUA : 	glColor3f(0, 1, 1); 		break;
        case BLACK : 	glColor3f(0, 0, 0); 		break;
        case BLUE :		glColor3f(0, 0, 1); 		break;
        case FUCHSIA :	glColor3f(1, 0, 1); 		break;
        case GRAY : 	glColor3f(0.5, 0.5, 0.5); 	break;
        case GREEN :	glColor3f(0, 0.5, 0); 		break;
        case LIME :		glColor3f(0, 1, 0); 		break;
        case MAROON :	glColor3f(0.5, 0, 0);	 	break;
        case NAVY :		glColor3f(0, 0, 0.5); 		break;
        case OLIVE :	glColor3f(0.5, 0.5, 0); 	break;
        case ORANGE :	glColor3f(1, 0.65, 0);	 	break;
        case PURPLE :	glColor3f(0.5, 0, 0.5); 	break;
        case RED :		glColor3f(1, 0, 0); 		break;
        case SILVER :	glColor3f(0.75, 0.75, 0.75);break;
        case TEAL :		glColor3f(0, 0.5, 0.5); 	break;
        case WHITE :	glColor3f(1, 1, 1);			break;
        case YELLOW :	glColor3f(1, 1, 0);			break;
     }
}
void graphic_set_color_3f(float red, float green, float blue) {
	
	glColor3f(red, green, blue);
}

void graphic_set_line_width(float width)
{
  glLineWidth(width);
}
