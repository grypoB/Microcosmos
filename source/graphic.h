/* Nom: graphic.c
 * Description: module pour les dessins Opengl
 * Date: 08.02.2014
 * version : 1.1
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */
 
#include "geometry.h"

#ifndef GRAPHIC_H
#define GRAPHIC_H

#define GRAPHIC_EMPTY  0
#define GRAPHIC_FILLED 1

typedef enum  {
AQUA, BLACK, BLUE, FUCHSIA, GRAY, GREEN, LIME, MAROON, NAVY, OLIVE, ORANGE, PURPLE, RED, SILVER, TEAL, WHITE, YELLOW
} GRAPHIC_COLOR;

typedef enum  {
CONTINUOUS, DASH_LINE, DISC
} DETAIL;

//draws a disk or a circle : dashed lines or continuous lines can be chosen
void graphic_draw_circle(POINT center, double radius, DETAIL detail);

//draws an heptagram
void graphic_draw_star(POINT center);

//draws a vector
void graphic_draw_vector(POINT center, VECTOR vector);

//sets the the color of generators and black holes
void graphic_set_color(GRAPHIC_COLOR color);

//sets the color of particles
void graphic_part_color(VECTOR speed);

//set the width of the line
void graphic_set_line_width(float width);


#endif
