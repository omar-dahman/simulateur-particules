/**
 * \file    optimization.c
 * \author  Othmane CHAOUI
 * \date    2026-03-27
 *
 * \brief   Optimization handling implementation
 * \details This file contains implementations of functions responsable
 *          of optimizing the simulation, which (for our group 22) is
 *          minimizing the maximum distance between any two particles
 *          at the first iteration exceeding T seconds.
 */

#include <stdio.h>
#include "optimization.h"

/**
 * \brief   Place particles at the center with null speed
 * \details This function positions all particles in the environment
 *          at the specified center coordinates and sets their speed
 *          to zero. This is used for the initial optimization case
 *          when T = 0.
 *
 * \param   e           environment as \a env
 * \param   center_x    x-coordinate of the center as \a float
 * \param   center_y    y-coordinate of the center as \a float
 * \return  nothing
 */
void place_at_center_null_speed(env e, float center_x, float center_y) {
    int env_n = get_n(e);

    for (int i = 0; i < env_n; i++) {
        particule p = get_particule(e, i);
        set_position(p, center_x, center_y);
        set_speed(p, 0.0, 0.0);
    }
}

/**
 * \brief   Set particle speeds to converge towards the center
 * \details This function adjusts the speed of each particle to point
 *          towards the specified center coordinates. The speed vectors
 *          are normalized to unit length. This is used for the optimization
 *          case when T > 0 to minimize particle spread over time.
 *
 * \param   e           environment as \a env
 * \param   center_x    x-coordinate of the center as \a float
 * \param   center_y    y-coordinate of the center as \a float
 * \return  nothing
 */
void converge_speed_to_center(env e, float center_x, float center_y) {
    int env_n = get_n(e);

    for (int i = 0; i < env_n; i++) {
        particule p = get_particule(e, i);
        float p_x = get_x(p);
        float p_y = get_y(p);
        
        float dx = center_x - p_x;
        float dy = center_y - p_y;
        
        // if particle is at the center
        if (p_x == center_x && p_y == center_y) {
            set_speed(p, 0.0, 0.0);
        }
        else {
            set_speed(p, dx, dy);
        }
    }
}

void optimize(env e, float t) {
    float env_w = get_w(e);
    float env_h = get_h(e);

    float center_x = env_w / 2.0;
    float center_y = env_h / 2.0;

    if (t == 0.0) {
        place_at_center_null_speed(e, center_x, center_y);
    }
    else {
        converge_speed_to_center(e, center_x, center_y);
    }
}