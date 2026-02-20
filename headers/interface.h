/**
 * \file    interface.h
 * \author  rayen.jalouali
 * \date    2026-02-19
 *
 * \brief   User interface and I/O management
 * \details This file contains declarations for functions that handle
 *          input/output operations, including printing particles and
 *          environments to standard output, reading user input from
 *          standard input, and generating PBM image files.
 */
#ifndef INTERFACE_H
#define INTERFACE_H

#include "environnement.h"

/**
 * \brief   Prints a particle's information
 * \details This function prints the particle's information
 *          to standard output
 *
 * \param   p   particle as \a particule
 * \return  nothing
 */
void print_particule(particule p);

/**
 * \brief   Prints an environment's information
 * \details This function prints the environment's information
 *          and all its particles to standard output
 *
 * \param   environnement   environment as \a env
 * \return  nothing
 */
void print_environnement(env environnement);

/**
 * \brief   Prints a message
 * \details This function prints a string to standard output.
 *          This file (interface.h) is the only file allowed to
 *          explicitly use standard output
 *
 * \param   message   string to print as \a char*
 * \return  nothing
 */
void print_message(char *message);

/**
 * \brief   Reads an integer from standard input
 * \details This function reads and returns an integer from
 *          standard input (e.g., entered by the user in the console).
 *          This file (interface.h) is the only file allowed to
 *          explicitly use standard input
 *
 * \return  integer read from standard input as \a integer
 */
int get_int();

/**
 * \brief   Reads a float from standard input
 * \details This function reads and returns a float from
 *          standard input (e.g., entered by the user in the console).
 *          This file (interface.h) is the only file allowed to
 *          explicitly use standard input
 *
 * \return  float read from standard input as \a float
 */
float get_float();

/**
 * \brief   Generates a PBM snapshot of the environment
 * \details This function writes to the file at path the contents
 *          of a PBM file of size wp x hp representing the environment.
 *          Path must be a valid absolute or relative path to a PBM
 *          image file
 *
 * \param   environnement   environment as \a env
 * \param   path            path to the output file as \a char*
 * \param   wp              width of the image in pixels as \a integer
 * \param   hp              height of the image in pixels as \a integer
 * \return  nothing
 */
void snapshot(env environnement, char *path, int wp, int hp);

/**
 * \brief   Generates an animated sequence of PBM images
 * \details This function writes t files containing PBM images
 *          of size wp x hp representing the environment over
 *          t iterations of movement. Path must be a valid absolute
 *          or relative path to an existing directory.
 *          The image corresponding to iteration i will be named
 *          env-i.pbm
 *
 * \param   environnement   environment as \a env
 * \param   path            path to the output directory as \a char*
 * \param   wp              width of the images in pixels as \a integer
 * \param   hp              height of the images in pixels as \a integer
 * \param   t               number of iterations as \a integer
 * \return  nothing
 */
void animate(env environnement, char *path, int wp, int hp, int t);

#endif
