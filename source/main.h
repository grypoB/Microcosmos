/* Nom: main.h
 * Description: 
 * Date: 04.02.2014
 * version : 1.1
 * auteur : PROG II
 */

#ifndef MAIN_H
#define MAIN_H

enum Mode {ERROR, FORCE, INTEGRATION, GRAPHIC, SIMULATION, MODE_UNSET};
typedef enum Mode MODE;

int main (int argc, char *argv[]);
MODE read_mode(char string[]);

#endif
