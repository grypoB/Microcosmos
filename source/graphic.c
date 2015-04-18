/* Nom: graphic.c
 * Description: module pour les dessins Opengl
 * Date: 17.04.2015
 * version : 1.0
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <Opengl/glu.h>
#else
#include <GL/glu.h>
#endif

#include "graphic.h"
#include "geometry.h"
#include "constantes.h"


// for drawing pseudo-circles 
#define DASH_TOTAL_SIDES 100
#define CIRCLE_SIDES 16

// for displaying the vector top point
#define VECTOR_RATIO 0.3

//draws a circle with specific detail (see enum DETAIL)
void graphic_draw_circle(POINT center, double radius, DETAIL detail)
{
	int i;
    double x, y, alpha;
    int nb_sides;
	
	switch(detail)
	{
		case CONTINUOUS :
            glBegin (GL_LINE_LOOP);
            nb_sides = CIRCLE_SIDES;
           
            break;

		case DASH_LINE :
            glBegin(GL_LINES);
            nb_sides = DASH_TOTAL_SIDES;
          
            break;
            
		case DISC :
            glBegin (GL_POLYGON);
			nb_sides = CIRCLE_SIDES;
			
            break;
	}

	for (i=0; i < nb_sides; i++)
    {
            alpha = i * 2. * M_PI / nb_sides;
            x = center.x + radius * cos (alpha);
            y = center.y + radius * sin (alpha);
            glVertex2f(x,y);
	}
	glEnd ();
}

//draws a vector
void graphic_draw_vector(POINT center, VECTOR vector)
{
	glBegin (GL_LINES);
	  glVertex2f (center.x, center.y);
      glVertex2f (center.x + vector.x*(1+VECTOR_RATIO), 
				  center.y +vector.y*(1+VECTOR_RATIO));
	glEnd ();
	
    // triangle at the end to make the vector an arrow
	glBegin (GL_POLYGON);
	  glVertex2f(center.x + vector.x * (1+VECTOR_RATIO), 
				 center.y + vector.y * (1+VECTOR_RATIO));
	  glVertex2f(center.x + vector.x - vector.y * VECTOR_RATIO, 
				 center.y + vector.y + vector.x * VECTOR_RATIO);
	  glVertex2f(center.x + vector.x + vector.y * VECTOR_RATIO, 
				 center.y + vector.y - vector.x * VECTOR_RATIO);
	glEnd ();

}

//draws a regular polygon of nb_sides and circumradius rayon 
void graphic_draw_polygon(POINT center, int nb_sides, int rayon)
{
	int i;
	float alpha = 0;
	
	glBegin (GL_POLYGON);
	
	for(i=0; i<nb_sides; i++)
	{
		  alpha = i * 2. * M_PI / nb_sides;
		  glVertex2f (center.x + rayon * cos (alpha), 
					  center.y + rayon * sin (alpha));
	}
	  
	glEnd ();
}

//sets the color of the element
void graphic_set_color(GRAPHIC_COLOR color) 
{
    switch (color) 
    {
        case AQUA :    glColor3f(0, 1, 1);          break;
        case BLACK :   glColor3f(0, 0, 0);          break;
        case BLUE :    glColor3f(0, 0, 1);          break;
        case FUCHSIA : glColor3f(1, 0, 1);          break;
        case GRAY :    glColor3f(0.5, 0.5, 0.5);    break;
        case GREEN :   glColor3f(0, 0.5, 0);        break;
        case LIME :    glColor3f(0, 1, 0);          break;
        case MAROON :  glColor3f(0.5, 0, 0);        break;
        case NAVY :    glColor3f(0, 0, 0.5);        break;
        case OLIVE :   glColor3f(0.5, 0.5, 0);      break;
        case ORANGE :  glColor3f(1, 0.65, 0);       break;
        case PURPLE :  glColor3f(0.5, 0, 0.5);      break;
        case RED :     glColor3f(1, 0, 0);          break;
        case SILVER :  glColor3f(0.75, 0.75, 0.75); break;
        case TEAL :    glColor3f(0, 0.5, 0.5);      break;
        case WHITE :   glColor3f(1, 1, 1);          break;
        case YELLOW :  glColor3f(1, 1, 0);          break;
     }
}

//sets the color to a specific RGB value (normalized format [0,1])
void graphic_set_color_3f(float red, float green, float blue) {
	
	glColor3f(red, green, blue);
}

//sets the width of all lines to draw
void graphic_set_line_width(float width)
{
  glLineWidth(width);
}
