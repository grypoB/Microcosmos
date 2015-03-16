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



void       graphic_draw_segment (float x1,
                                 float y1,
                                 float x2, 
                                 float y2);

void       graphic_draw_rectangle (float xc,
			           float yc, 
			           float width,
                                   float height,
                                   int   filled);

void graphic_draw_particule(POINT center, double radius, int filled); //PAS FINI

void graphic_draw_generateur(POINT center, double radius, int filled); //PAS FINI

void graphic_draw_bckH(POINT center); //PAS FINI

void       graphic_set_color3f(float r,
                             float g, 
                             float b);

void       graphic_set_color3fv(float color[3]);


void       graphic_set_line_width(float width);


#endif
