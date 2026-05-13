/**
 * \file    interface.h
 * \author  Omar DAHMAN
 * \date    2026-04-04
 *
 * \brief   User interface and I/O management
 * \details This file contains declarations for functions that handle
 *          input/output operations, including printing particles and
 *          environments to standard output, reading user input from
 *          standard input, and generating PBM image files.
 *
 * \note MODIFIED FOR LOT E (Tâche E.2):
 *   - print_particule: also prints the z-coordinate and vz.
 *   - print_environnement: also prints the depth d.
 *   - snapshot: projects 3D positions onto the XY plane.
 *   - animate: function signature unchanged; now uses updated snapshot internally
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "environnement.h"
#include "camera.h"

/**
 * \brief   Prints a particle's information
 * \details Prints the particle's position (x, y, z) and velocity (vx, vy, vz)
 *          to standard output.
 * \note MODIFIED FOR LOT E (Tâche E.2): now prints z and vz.
 *
 * \param   p   particle as \a particule
 */
void print_particule(particule p);

/**
 * \brief   Prints an environment's information
 * \details Prints the environment's parameters (including depth d) and all
 *          its particles to standard output.
 * \note MODIFIED FOR LOT E (Tâche E.2): now prints depth d.
 *
 * \param   environnement   environment as \a env
 */
void print_environnement(env environnement);

/**
 * \brief   Prints a message to standard output
 * \details This file (interface.h) is the only file allowed to use standard output.
 *
 * \param   message   string to print as \a char*
 */
void print_message(char *message);

/**
 * \brief   Reads an integer from standard input
 * \details This file (interface.h) is the only file allowed to use standard input.
 * \return  integer read from standard input
 */
int get_int(void);

/**
 * \brief   Reads a float from standard input
 * \details This file (interface.h) is the only file allowed to use standard input.
 * \return  float read from standard input
 */
float get_float(void);

/**
 * \brief   Generates a PBM snapshot of the environment 
 * \details Writes a PBM file of size wp x hp representing the environment.
 *          3D positions are projected onto the XY plane (z-axis ignored).
 *          Path must be a valid absolute or relative path to a PBM file.
 * \note MODIFIED FOR LOT E (Tâche E.2): projection now uses the 3D position
 *       (x, y) only; z is ignored for the flat PBM output. The camera tâche (E.3)
 *       will handle proper 3D rendering.
 *
 * \param   environnement   environment as \a env
 * \param   path            path to the output file as \a char*
 * \param   wp              width  of the image in pixels as \a integer
 * \param   hp              height of the image in pixels as \a integer
 * \param cam               the camera instance
 * 
 * modified for tâche e.4
 */
void snapshot(env environnement, char *path, int wp, int hp, Camera* cam);

/**
 * \brief   Generates an animated sequence of PBM images
 * \details Writes t PBM files of size wp x hp representing the environment
 *          over t movement iterations. Path must be a valid path to an
 *          existing directory. Image for iteration i is named env-i.pbm.
 *
 * \param   environnement   environment as \a env
 * \param   path            path to the output directory as \a char*
 * \param   wp              width  of the images in pixels as \a integer
 * \param   hp              height of the images in pixels as \a integer
 * \param   t               number of iterations as \a integer
 * \param cam               the camera instance
 * 
 * modified for tâche e.4
 */
void animate(env environnement, char *path, int wp, int hp, int t, Camera* cam);

#endif 
