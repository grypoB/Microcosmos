/* Nom: main.h
 * Description: module sous-système de contrôle: gestion du dialogue avec l'utilisateur et interface graphique
 * Date: 08.02.2014
 * version : 1.0
 * responsable du module : Pauline Maury Laribière
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef MAIN_H
#define MAIN_H

enum Mode {ERROR, FORCE, INTEGRATION, GRAPHIC, SIMULATION, MODE_UNSET};
typedef enum Mode MODE;

int main (int argc, char *argv[]);
MODE read_mode(char string[]);

#endif
