#ifndef INTERFACE_H
#define INTERFACE_H

#include "environnement.h"

/*
 * Function print_particule
 * Takes a particle as input, returns nothing and prints the
 * particle's information to standard output.
 */
void print_particule(particule p);

/*
 * Function print_environnement
 * Takes an environment as input, returns nothing and prints the
 * environment's information and all its particles to standard output.
 */
void print_environnement(environnement env);

/*
 * Function print_message
 * Takes a string as input, returns nothing and prints it
 * to standard output.
 * This file (interface.h) is the only file allowed to explicitly
 * use standard output.
 */
void print_message(char *message);

/*
 * Function get_int
 * Takes no input and returns an integer from standard input
 * (e.g., entered by the user in the console).
 * This file (interface.h) is the only file allowed to explicitly
 * use standard input.
 */
int get_int();

/*
 * Function get_float
 * Takes no input and returns a float from standard input
 * (e.g., entered by the user in the console).
 * This file (interface.h) is the only file allowed to explicitly
 * use standard input.
 */
float get_float();

/*
 * Function snapshot
 * Takes an environment, a string path and two integers wp and hp as input,
 * returns nothing and writes to the file at path the contents of a
 * PBM file of size wp x hp representing the environment
 * (following the rules described in the assignment).
 * Path must be a valid absolute or relative path to a PBM image file.
 */
void snapshot(environnement env, char *path, int wp, int hp);

/*
 * Function animate
 * Takes an environment, a string path, two integers wp and hp
 * and an integer t as input, returns nothing and writes t files
 * containing PBM images of size wp x hp representing the environment
 * over t iterations of movement.
 * Path must be a valid absolute or relative path to an existing directory.
 * The image corresponding to iteration i will be named env-i.pbm.
 */
void animate(environnement env, char *path, int wp, int hp, int t);

#endif /* INTERFACE_H */
