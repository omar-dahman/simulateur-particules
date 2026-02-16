/**
 * \file    environnement.h
 * \author  Othmane CHAOUI
 * \date    2026-02-16
 *
 * \brief   Particles management and handling
 * \details This file contains declarations for functions that will 
 *          allow managing the set of particles, their movements, 
 *          their interactions and the interactions between the
 *          particles and the edges.
 */
#ifndef _ENVIRONNEMENT_H
#define _ENVIRONNEMENT_H

#include "particules.h" // for the type "particule" in get_particule function

/**
 * \typedef \a env
 * \brief   Type for an \b environment
 * \details This type is used for variables representing an environment of particles
 */
typedef struct env_structure* env;

/**
 * \brief   Creates an environment of particles
 * \details This function performs a memory allocation
 *          for environment of size w*h for n particles.
 *          The particles are all placed at the center of
 *          the environment with a speed that sends them to the right
 *
 * \param   n   number of particles as \a integer
 * \param   w   width of the environment as \a float
 * \param   h   height of the environment as \a float
 * \param   r   interaction radius as \a float
 * \param   dt  iterations duration as \a float
 * \return  environment of particles as \a env
 */
env create_environnement(int n, float w, float h, float r, float dt);

/**
 * \brief    Frees environment memory
 * \details  This function frees the allocated memory by the environment
 *
 * \param   e   environment as \a env
 * \return  nothing
 */
void free_environnement(env e);

/**
 * \brief   Returns number of particles
 * \details This function returns the number of paricles
 *          in an environment
 *
 * \param   e   environment as \a env
 * \return  number of particules as \a integer
 */
int get_n(env e);

/**
 * \brief   Returns a particle
 * \details This function returns the particle at
 *          the i-th position
 * 
 * \param   e   environment as \a env
 * \param   i   number of the particle as \a integer
 * \return  i-th particle of the environment as \a particule
 */
particule get_particule(env e, int i);

/**
 * \brief   Returns width of environment
 * \details This function returns the width of an environment
 *
 * \param   e   environment as \a env
 * \return  The width of the environment as \a float
 */
float get_w(env e);

/**
 * \brief   Returns height of environment
 * \details This function returns the height of an environment
 *
 * \param   e   environment as \a env
 * \return  The height of the environment as \a float
 */
float get_h(env e);

/**
 * \brief   Returns duration of iterations
 * \details This function returns the duration of iterations
 *          of movements in an environment
 *
 * \param   e   environment as \a env
 * \return  The duration of iterations in the environment as \a float
 */
float get_dt(env e);

/**
 * \brief   Returns duration of iterations
 * \details This function returns the radius of interactions
 *          of particles
 *
 * \param   e   environment as \a env
 * \return  The duration of iterations in the environment as \a float
 */
float get_r(env e);

/**
 * \brief   moves a particle
 * \details This function performs a movement iteration on the
 *          particles of an environment
 *
 * \param   e environment as \a env
 * \return  nothing
 */
void move_particules(env e);

#endif