/*------------------------------------------------------------------------*/
#define _USE_MATH_DEFINES
#define NB_COTES 50

#include <stdio.h>
#include <math.h>
#include <GL/glu.h>
#include "graphic.h"
#include "geometry.h"
/*=================================================================*/
/* Fonctions pour afficher un segment, un rectangle ou un cercle.  */
/*								   */
/* Pour le segment, on fournit les coordonnnees des deux extremites*/
/*                                                                 */
/* Pour le rectangle, on fournit les coordonnees de son centre et  */
/* de sa taille (width, height).                                   */
/*                                                                 */
/* Pour le cercle, on fournit les coordonnees de son centre et son */
/* rayon.                                                          */
/*                                                                 */
/* Le rectangle et le cercle peuvent etre creux ou pleins (filled) */
/* suivant la valeur du dernier argument (GRAPHIC_FILLED ou bien   */
/* GRAPHIC_EMPTY).                                                 */
/*=================================================================*/

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

/*-----------------------------------------------------------------------*/

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
/*-----------------------------------------------------------------------*/

void graphic_draw_particule(POINT center, double radius, int filled) //a revoir
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
