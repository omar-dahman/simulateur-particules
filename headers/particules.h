/**
 * \file particules.h
 * \author Omar DAHMAN
 * \brief Individual particle management
 *
 * This file allows managing each particle individually.
 * Functions defined:
 *  - \b create_particule allocates memory and creates a particle.
 *  - \b free_particule frees the memory related to a particle.
 *  - \b get_x returns the x-coordinate of the particle.
 *  - \b get_y returns the y-coordinate of the particle.
 *  - \b get_z returns the z-coordinate of the particle.       [ADDED FOR LOT E - Tâche E.2]
 *  - \b get_vx returns the horizontal speed of the particle.
 *  - \b get_vy returns the vertical speed of the particle.
 *  - \b get_vz returns the depth speed of the particle.       [ADDED FOR LOT E - Tâche E.2]
 *  - \b set_position sets the position of the particle.
 *  - \b set_speed sets the speed of the particle.
 *  - \b move moves the particle.
 *  - \b distanceve returns the Euclidean distance between the positions of two particles.
 *  - \b get_pos returns the position of the particle as a vec3 [ADDED FOR LOT E - Tâche E.2]
 *  - \b get_vel returns the velocity of the particle as a vec3 [ADDED FOR LOT E - Tâche E.2]
 */

#ifndef PARTICULES_H
#define PARTICULES_H

#include "vector.h" /* ADDED FOR LOT E (Tâche E.2) : needed for vec3 */

/**
 * \typedef particule
 * Type representing a particle.
 */
typedef struct particule_s *particule;

/**
 * \brief Creates and returns a particle located at (x, y, z) with speed vector (vx, vy, vz).
 * If the speed vector is not unitary, it is normalized.
 * \param x  x-coordinate of the particle.
 * \param y  y-coordinate of the particle.
 * \param vx horizontal speed of the particle.
 * \param vy vertical speed of the particle.
 * \return A new particle.
 *
 * \note  Signature MODIFIED FOR LOT E (Tâche E.2): added z and vz parameters for 3D.
 */
particule create_particule(float x, float y, float z, float vx, float vy, float vz);

/**
 * \brief Frees the memory of particle \a p.
 * \attention The particle p must have been created with create_particule
 * and must not already have been destroyed with free_particule.
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
 * \brief Returns the z-coordinate of particle \a p.
 * \param p A particle.
 * \return The z-coordinate of particle \a p.
 * \note ADDED FOR LOT E (Tâche E.2) : 3D extension.
 */
float get_z(particule p);

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
 * \brief Returns the depth speed of particle \a p.
 * \param p A particle.
 * \return The depth (z-axis) speed of particle \a p.
 * \note ADDED FOR LOT E (Tâche E.2) : 3D extension.
 */
float get_vz(particule p);

/**
 * \brief Replaces the position of particle \a p with (x, y, z).
 * \param p A particle.
 * \param x x-coordinate.
 * \param y y-coordinate.
 * \param z z-coordinate.
 * \note Signature MODIFIED FOR LOT E (Tâche E.2): added z parameter.
 */
void set_position(particule p, float x, float y, float z);

/**
 * \brief Replaces the speed of particle \a p with (vx, vy, vz).
 * If the vector is not unitary, it is normalized.
 * \param p  A particle.
 * \param vx horizontal speed.
 * \param vy vertical speed.
 * \param vz depth speed.
 * \note Signature MODIFIED FOR LOT E (Tâche E.2): added vz parameter.
 */
void set_speed(particule p, float vx, float vy, float vz);

/**
 * \brief Modifies the position of the particle,
 * as if it were moving in a straight line for \a dt seconds.
 * \param p  A particle.
 * \param dt Duration of the particle movement.
 */
void move(particule p, float dt);

/**
 * \brief Returns the Euclidean distance between the positions of p1 and p2.
 * \param p1 A particle.
 * \param p2 Another particle.
 * \return The Euclidean distance between the positions of the particles.
 */
float distanceve(particule p1, particule p2);

/**
 * \brief Returns the position of particle \a p as a vec3.
 * \param p A particle.
 * \return Position vector (x, y, z).
 * \note ADDED FOR LOT E (Tâche E.2) : function using the vector library.
 */
vec3 get_pos(particule p);

/**
 * \brief Returns the velocity of particle \a p as a vec3.
 * \param p A particle.
 * \return Velocity vector (vx, vy, vz).
 * \note ADDED FOR LOT E (Tâche E.2) : function using the vector library.
 */
vec3 get_vel(particule p);

#endif 
