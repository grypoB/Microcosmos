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

void graphic_draw_particule(POINT center, double radius);

void graphic_draw_generateur(POINT center, double radius, VECTOR vector);

void graphic_draw_bckH(POINT center);

void graphic_set_color3f(float r,  float g, float b);

void graphic_set_line_width(float width);


#endif
