/**
 * \file    optimization.h
 * \author  Othmane CHAOUI
 * \date    2026-03-27
 *
 * \brief   Optimization handling declaration
 * \details This file contains declarations of one funcntion responsable
 *          of optimizing the simulation, which (for our group 22) is
 *          minimizing the maximum distance between any two particles 
 *          at the first iteration exceeding T seconds.
 */

#ifndef _OPTIMIZATION_H
#define _OPTIMIZATION_H

#include "environnement.h"

/**
 * \brief   Optimize the simulation
 * \details This function performs the optimization by changing
 *          the speed of the particles in a way that minimizes
 *          the maximum distance between the particles. Therfore,
 *          we identify 2 cases:
 *          T = 0 : the particles are placed at the center with
 *                  a null speed
 *          T > 0 : we change the speed of the particles converging to the
 *                  center of the environment
 *
 * \param   e   environment as \a env
 * \param   t   duration after which applies the optimization as \a float
 * \return  nothing
 */
void optimize(env e, float t);

#endif