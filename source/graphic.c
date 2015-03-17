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


void graphic_draw_particule(POINT center, double radius) 
{ 																	
  int i;
  const int SIDES = 50;

  glBegin (GL_POLYGON);
  for (i=0; i < SIDES; i++)
    {
      float alpha = i * 2. * M_PI / SIDES;
	  float x = center.x + radius * cos (alpha);
	  float y = center.y + radius * sin (alpha);
      glVertex2f(x,y);
    }

  glEnd ();
}

void graphic_draw_generateur(POINT center, double radius, VECTOR vector)
{ 
   glBegin (GL_POINT);
    glVertex2f (center.x, center.y);
   glEnd ();   
  
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

void graphic_draw_bckH(POINT center)  //point avec cercle en pointillé autour
{
	int i;
	const int SIDES = 50;
   
	glBegin (GL_POINT);
    glVertex2f (center.x, center.y);
    glEnd ();    
	
	glBegin (GL_LINE_LOOP);
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

void graphic_set_color3f(float r, float g, float b)
{
  glColor3f(r, g, b);
}

void graphic_set_line_width(float width)
{
  glLineWidth(width);
}
