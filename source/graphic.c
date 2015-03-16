/* Nom: graphic.c
 * Description: module pour les dessins Opengl
 * Date: 08.02.2014
 * version : 1.1
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#define _USE_MATH_DEFINES
#define NB_COTES 50

#include <stdio.h>
#include <math.h>
#include <GL/glu.h>
#include "graphic.h"
#include "geometry.h"


void graphic_draw_particule(POINT center, double radius, int filled) //a revoir  
{ 																	//aussi couleur en fonction de la vitesse (ici ou laisser dans sim.c)
  int i;
  const int SIDES = 50;

  if (filled == GRAPHIC_FILLED)
    glBegin (GL_POLYGON);
  else
    glBegin (GL_LINE_LOOP);
  
  for (i=0; i < SIDES; i++)
    {
      float alpha = i * 2. * M_PI / SIDES;
	  float x = center.x + radius * cos (alpha);
	  float y = center.y + radius * sin (alpha);
      glVertex2f(x,y);
    }

  glEnd ();
}

void graphic_draw_generateur(POINT center, double radius, int filled)   //a revoir
{ 
  int i;
  const int SIDES = 50;

  if (filled == GRAPHIC_FILLED)
    glBegin (GL_POLYGON);
  else
    glBegin (GL_LINE_LOOP);
  
  for (i=0; i < SIDES; i++)
    {
      float alpha = i * 2. * M_PI / SIDES;
	  float x = center.x + radius * cos (alpha);
	  float y = center.y + radius * sin (alpha);
      glVertex2f(x,y);
    }

  glEnd ();
}

void graphic_draw_bckH(POINT center)  //a revoir
{
	glBegin (GL_POINT);

    glVertex2f (center.x, center.y);

    glEnd ();
}

void graphic_set_color3f(float r,
                       float g,
                       float b)
{
  glColor3f(r, g, b);
}

void graphic_set_color3fv(float color[3])
{
  glColor3fv(color);
}

void graphic_set_line_width(float width)
{
  glLineWidth(width);
}


 /* SERA PEUT ETRE UTILE PLUS TARD
void graphic_draw_segment (float x1,
                           float y1,
                           float x2,
                           float y2)

{ 
    glBegin (GL_LINES);

    glVertex2f (x1, y1);
    glVertex2f (x2, y2);

    glEnd ();
}

-----------------------------------------------------------------------

void graphic_draw_rectangle (float xc,
			     float yc,
			     float width,
                             float height,
                             int   filled)

{   if (filled == GRAPHIC_FILLED)
      glBegin (GL_POLYGON);
    else
      glBegin (GL_LINE_LOOP);

    glVertex2f (xc+width/2, yc+height/2);
    glVertex2f (xc-width/2, yc+height/2);
    glVertex2f (xc-width/2, yc-height/2);
    glVertex2f (xc+width/2, yc-height/2);

    glEnd ();
}
-----------------------------------------------------------------------*/
