/* Nom: graphic.c
 * Description: module pour les dessins Opengl
 * Date: 08.02.2014
 * version : 1.1
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#define NB_COTES 50

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


//draws a circle : dashed lines or continuous lines can be chosen
void graphic_draw_circle(POINT center, double radius, DETAIL detail)
{
	int i;
	//int j = 0;
	//int k = 0;
	const int SIDES = 50;

	glBegin (GL_LINE_LOOP);
	
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
						if(i%2==0) graphic_set_color(GREEN);
						else graphic_set_color(WHITE); 
						 
						float alpha = i * 2. * M_PI / SIDES;
						float x = center.x + radius * cos (alpha);
						float y = center.y + radius * sin (alpha);
						glVertex2f(x,y);
					}
					break;
					
					/*for (i=0; i < SIDES; i++)
					{
						while(j<2)
						{
							graphic_set_color(GREEN);   
							float alpha = i * 2. * M_PI / SIDES;
							float x = center.x + radius * cos (alpha);
							float y = center.y + radius * sin (alpha);
							glVertex2f(x,y);
							i++;
							j++;
						}
						while(k<2)
						{
							graphic_set_color(WHITE);    //couleur du fond d'écran
							float alpha = i * 2. * M_PI / SIDES;
							float x = center.x + radius * cos (alpha);
							float y = center.y + radius * sin (alpha);
							glVertex2f(x,y);
							i++;
							k++;
						}
						j=0;
						k=0;
					}	*/				
					
					
					/*2
					 * for (i=0; i < SIDES; i++)
					{
						float alpha = i * 2. * M_PI / SIDES;
						float x = center.x + radius * cos (alpha);
						float y = center.y + radius * sin (alpha);
						glVertex2f(x,y);
					}
					
					graphic_set_color(WHITE);    //couleur du fond d'écran
					for (j=3; j < SIDES; j+=3)
					{
						float alpha = j * 2. * M_PI / SIDES;
						float x = center.x + radius * cos (alpha);
						float y = center.y + radius * sin (alpha);
						glVertex2f(x,y);
					}
					break;			*/
					
					
					/*3
					 * for (j=0; j < SIDES; j++)
					{
						for (i=0; i < POINTILLE; i++)
						{
							float alpha = i * 2. * M_PI / SIDES;
							float x = center.x + RBLACK * cos (alpha);
							float y = center.y + RBLACK * sin (alpha);
							glVertex2f(x,y);
						}
						j+=POINTILLE;
					}*/
					break;
					
		default : printf("Wrong details in %s\n",__func__);
					break;
	}

	glEnd ();
}

void graphic_draw_disc(POINT center, double radius)
{
	const int SIDES = 50;
	int i;
	
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


//draws a vector
void graphic_draw_vector(POINT center, VECTOR vector)  //a revoir (magic numbers ??)
{
	glBegin (GL_LINES);
	  glVertex2f (center.x, center.y);
      glVertex2f (center.x + vector.x, center.y +vector.y);
	glEnd ();
	
	glBegin (GL_TRIANGLES);
	  glVertex2f(center.x + vector.x - 2, center.y +vector.y);
	  glVertex2f(center.x + vector.x, center.y +vector.y + 2);
	  glVertex2f(center.x + vector.x + 2, center.y +vector.y);
	glEnd ();

}

void graphic_draw_star(POINT center) //surement taille a adapter (magic numbers ??)
{
	glBegin (GL_LINE_LOOP);
	  glVertex2f(center.x , center.y + 5 );  		//1
	  glVertex2f(center.x + 0.6, center.y + 1.5); 	//2
	  glVertex2f(center.x + 4, center.y + 3);    	//3
	  glVertex2f(center.x + 1.6, center.y + 0.3);   //4
	  glVertex2f(center.x + 5, center.y - 1);   	//5
	  glVertex2f(center.x + 1.3, center.y -0.8);  	//6
	  glVertex2f(center.x + 2.2, center.y -5);  	//7
	  glVertex2f(center.x , center.y - 1.8);  		//8 
	  glVertex2f(center.x - 2.2, center.y - 5);     //9
	  glVertex2f(center.x -1.3, center.y - 0.8); 	//10
	  glVertex2f(center.x - 5, center.y - 1);   	//11
	  glVertex2f(center.x - 1.6, center.y + 0.3);   //12
	  glVertex2f(center.x - 4, center.y + 3);   	//13
	  glVertex2f(center.x -0.6, center.y + 1.5); 	//14
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
