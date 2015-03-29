/* Nom: graphic.c
 * Description: module pour les dessins Opengl
 * Date: 08.02.2014
 * version : 1.1
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#define _USE_MATH_DEFINES
#define NB_COTES 50
#define ARROW 2 //arbitraire
#define POINTILLE 3

#include <stdio.h>
#include <math.h>
#include <GL/glu.h>
#include "graphic.h"
#include "geometry.h"
#include "constantes.h"


//draws a circle : dashed lines or continuous lines can be chosen
void graphic_circle(POINT center, double radius, DETAIL detail)
{
	int i;
	const int SIDES = 50;

	glBegin (GL_POLYGON);
	
	switch(detail)
	{
		case CONTINUOUS :
					for (i=0; i < SIDES; i++)
					{
						float alpha = i * 2. * M_PI / SIDES;
						float x = center.x + radius * cos (alpha);
						float y = center.y + radius * sin (alpha);
						glVertex2f(x,y);
					}
					break;
					
		case DASH_LINE :
					for (i=0; i < SIDES; i++)
					{
						for (i=0; i < POINTILLE; i++)
						{
							float alpha = i * 2. * M_PI / SIDES;
							float x = center.x + RBLACK * cos (alpha);
							float y = center.y + RBLACK * sin (alpha);
							glVertex2f(x,y);
						}
		
							i+=POINTILLE;
					}
		glEnd ();
	}
}

void graphic_draw_point(POINT center)
{
	glBegin (GL_POINT);
	  glVertex2f (center.x, center.y);
	glEnd (); 
}


//draws a vector
void graphic_draw_vector(POINT center, double radius, VECTOR vector)
{
	glBegin (GL_LINES);                            //barre de la flèche
	  glVertex2f (center.x, center.y);
      glVertex2f (center.x + vector.x, center.y +vector.y);
	glEnd ();
	
	glBegin (GL_LINES);                           //pour pointe de la flèche
	  glVertex2f (center.x + vector.x, center.y + vector.y);
      glVertex2f (center.x + vector.x - ARROW, center.y + vector.y - ARROW);
	glEnd ();
  
	glBegin (GL_LINES);                           //pour pointe de la flèche
      glVertex2f (center.x + vector.x, center.y + vector.y);
      glVertex2f (center.x + vector.x + ARROW, center.y + vector.y + ARROW);
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

void graphic_part_color(VECTOR speed) {
	
	float red   = linear_interpolation(vector_norm(speed), 0, 0 , MAX_VITESSE, 1  );
	float green = linear_interpolation(vector_norm(speed), 0, 0 , MAX_VITESSE, 0.2);
	float blue  = linear_interpolation(vector_norm(speed), 0, 0 , MAX_VITESSE, 0.2);
	
	glColor3f(red, green, blue);
}

void graphic_set_line_width(float width)
{
  glLineWidth(width);
}
