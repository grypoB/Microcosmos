/* Nom: graphic.c
 * Description: module pour les dessins Opengl
 * Date: 17.04.2015
 * version : 1.0
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */
 
#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "geometry.h"

/* Do not forget to set the color before drawing anything */


// set of predefined color to use (see graphic_set_color)
typedef enum  {
AQUA, BLACK, BLUE, FUCHSIA, GRAY, GREEN, LIME, MAROON, NAVY, OLIVE, ORANGE,
PURPLE, RED, SILVER, TEAL, WHITE, YELLOW
} GRAPHIC_COLOR;

// different display possibilities for a circle in graphic_draw_circle
typedef enum  {
    CONTINUOUS, // continuous line
    DASH_LINE,  // dashed line
    DISC        // filled circle (hence a disc)
} DETAIL;

//draws a circle with specific detail (see enum DETAIL)
void graphic_draw_circle(POINT center, double radius, DETAIL detail);

//draws a regular polygon of nb_sides and circumradius rayon 
void graphic_draw_polygon(POINT center, int nb_sides, int rayon);

//draws a vector
void graphic_draw_vector(POINT center, VECTOR vector);

//set coloring color to a predefined color (see enum GRAPHIC_COLOR)
void graphic_set_color(GRAPHIC_COLOR color);

//sets the color to a specific RGB value (normalized format [0,1])
void graphic_set_color_3f(float red, float green, float blue);

//sets the width of all lines to draw
void graphic_set_line_width(float width);

#endif
