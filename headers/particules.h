#ifndef PARTICULES_H
#define PARTICULES_H

/**
 * \file particules.h
 * \brief Individual particle management
 *
 * This file allows managing each particle individually.
 * 5 functions are defined:
 *  - \b create_particule allocates memory and creates a particle.
 *  - \b free_particule frees the memory related to a particle.
 *  - \b get_x returns the x-coordinate of the particle.
 *  - \b get_y returns the y-coordinate of the particle.
 *  - \b get_vx returns the horizontal speed of the particle.
 *  - \b get_vy returns the vertical speed of the particle.
 *  - \b set_speed sets the speed of the particle.
 *  - \b move moves the particle.
 *  - \b distanceve returns the Euclidean distance between the positions of two particles.
 */

/**
 * \typedef particule
 * Type representing a particle.
 */
typedef struct particule_s *particule;

/**
 * \brief Creates and returns a particle located at ( \a x, \a y ) with speed vector ( \a vx, \a vy ).
 * If the speed vector is not unitary, it is normalized.
 * \param x x-coordinate of the particle.
 * \param y y-coordinate of the particle.
 * \param vx horizontal speed of the particle.
 * \param vy vertical speed of the particle.
 * \return A new particle located at ( \a x, \a y ) with speed vector ( \a vx, \a vy ).
 */
particule create_particule(float x, float y, float vx, float vy);

/**
 * \brief Frees the memory of particle \a p.
 * \attention The particle p must have been created with the function \a create_particule
 * and must not already have been destroyed with the function \a free_particule.
 * \param p A particle to destroy.
 */
void free_particule(particule p);

/**
 * \brief Returns the x-coordinate of particle \a p.
 * \param p A particle.
 * \return The x-coordinate of particle \a p.
 */
float get_x(particule p);

/**
 * \brief Returns the y-coordinate of particle \a p.
 * \param p A particle.
 * \return The y-coordinate of particle \a p.
 */
float get_y(particule p);

/**
 * \brief Returns the horizontal speed of particle \a p.
 * \param p A particle.
 * \return The horizontal speed of particle \a p.
 */
float get_vx(particule p);

/**
 * \brief Returns the vertical speed of particle \a p.
 * \param p A particle.
 * \return The vertical speed of particle \a p.
 */
float get_vy(particule p);

/**
 * \brief Replaces the position of particle \a p with ( \a x, \a y ).
 * \param p A particle.
 * \param x x-coordinate of the particle.
 * \param y y-coordinate of the particle.
 */
void set_position(particule p, float x, float y);

/**
 * \brief Replaces the speed of particle \a p with ( \a vx, \a vy ).
 * If the vector is not unitary, it is normalized.
 * \param p A particle.
 * \param vx horizontal speed of the particle.
 * \param vy vertical speed of the particle.
 */
void set_speed(particule p, float vx, float vy);

/**
 * \brief Modifies the position of the particle,
 * as if it were moving in a straight line for \a dt seconds.
 * \param p A particle.
 * \param dt Duration of the particle movement.
 */
void move(particule p, float dt);

/**
 * \brief Returns the Euclidean distance between the positions of \a p1 and \a p2.
 * \param p1 A particle.
 * \param p2 Another particle.
 * \return The Euclidean distance between the positions of the particles.
 */
float distanceve(particule p1, particule p2);
#endif