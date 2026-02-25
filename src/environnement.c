/**
 * \file    environnement.c
 * \author  Othmane CHAOUI
 * \date    2026-02-25
 *
 * \brief   Particles management and handling (Implementation)
 * \details This file contains implementtation of functions declared
 *          in \b envionnement.h
 */

#include <stdio.h>
#include <stdlib.h>
#include "environnement.h"
#include "distributions.h"

/**
 * \struct env_structure
 * \brief environment structure
 * \details This structure represents the implementation
 *          of a type for environment
 */
struct env_structure {
    float w;                    /**< width of the environment */
    float h;                    /**< height of the environment */
    int n;                      /**< number of particles in the environment */
    float r;                    /**< radius of interactions in the environment */
    float dt;                   /**< movement duration in the environment */
    particule* particules;      /**< table of particles in the environment */
};

env create_environnement(int n, float w, float h, float r, float dt) {
    env e;

    e = malloc(sizeof(struct env_structure));
    if (!e) {
        return NULL;
    }
    
    e -> w = w;
    e -> h = h;
    e -> n = n;
    e -> r = r;
    e -> dt = dt;
    
    e -> particules = malloc(n * sizeof(particule));
    if (!e -> particules) {
        free(e);
        return NULL;
    }
    
    for (int i = 0; i < n; i++) {
        e -> particules[i] = create_particule(w / 2, h / 2, 1.0, 0.0);
    }

    return e;
}

void free_environnement(env e) {
    int env_n = get_n(e);
    
    for (int i = 0; i < env_n; i++) {
        free_particule(e -> particules[i]);
    }
    
    free(e->particules);
    free(e);
}

int get_n(env e) {
    return e -> n;
}

particule get_particule(env e, int i) {
    if (i < 0 || i >= e -> n) {
        return NULL;
    }
    
    return e -> particules[i];
}

float get_w(env e) {
    return e -> w;
}

float get_h(env e) {
    return e -> h;
}

float get_dt(env e) {
    return e -> dt;
}

float get_r(env e) {
    return e -> r;
}

/**
 * \brief   border collision handler
 * \details This function handles the collisions of particles with the borders of the environment,
 *          and updates their positions accordinally
 *
 * \param   p           particle as \a particule
 * \param   env_w       width of the environment as \a float
 * \param   env_h       height of the environment as \a float
 * \param   old_p_x     old \b p's X-coordinates as \a float
 * \param   old_p_y     old \b p's Y-coordinates as \a float
 * \param   new_p_x     new \b p's X-coordinates as \a float
 * \param   new_p_y     new \b p's Y-coordinates as \a float
 * \return  nothing
 */
void border_collision_handler(particule p, float env_w, float env_h, float new_p_x, float new_p_y) {
    float p_vx = get_vx(p);
    float p_vy = get_vy(p);
    float final_p_x = new_p_x;
    float final_p_y = new_p_y;
    
    if (new_p_x >= env_w) { // right border
        final_p_x = 2 * env_w - new_p_x;
        p_vx = -p_vx;
    }
    else if (new_p_x <= 0) { // left border
        final_p_x = -new_p_x;
        p_vx = -p_vx;
    }
    
    if (new_p_y >= env_h) { // top border
        final_p_y = 2 * env_h - new_p_y;
        p_vy = -p_vy;
    }
    else if (new_p_y <= 0) { // bottom border
        final_p_y = -new_p_y;
        p_vy = -p_vy;
    }
    
    set_position(p, final_p_x, final_p_y);
    set_speed(p, p_vx, p_vy);
}    

/**
 * \brief   find neighbor particles and barycenter
 * \details This function finds the neighbor particles to \b p and the barycenter coordinates
 *
 * \param   e               environment as \a env
 * \param   p               particle as \a particule
 * \param   i               the number of the current particle \a int
 * \param   env_r           radius of the environment as \a float
 * \param   env_n           number of particles in the environment as \a int
 * \param   barycenter_x    pointer to barycenter X-coordiantes as \a float*
 * \param   barycenter_y    pointer to barycenter Y-coordiantes as \a float*
 * \return  the number of neighbors as \a int
 */
int find_neighbors_and_barycenter(env e, particule p, int i, float env_r, int env_n, float* barycenter_x, float* barycenter_y) {
    int number_of_neighbors = 0;
    *barycenter_x = 0;
    *barycenter_y = 0;
    
    for (int j = 0; j < env_n; j++) {
        if (i == j) {
            continue;
        }
        
        particule p2 = get_particule(e, j);
        float distance = distanceve(p, p2);
        
        if (distance <= env_r) {
            *barycenter_x += get_x(p2);
            *barycenter_y += get_y(p2);
            number_of_neighbors++;
        }
    }
    
    return number_of_neighbors;
}

/**
 * \brief   particles collision handler
 * \details This function handles the collisions between particles and calculates the barycenter coordinates
 *
 * \param   p                       particle as \a particule
 * \param   number_of_neighbors     number of neighbor particles as \a int
 * \param   barycenter_x            pointer to barycenter X-coordiantes as \a float*
 * \param   barycenter_y            pointer to barycenter Y-coordiantes as \a float*
 * \return  nothing
 */
void particles_collision_handler(particule p, int number_of_neighbors, float *barycenter_x, float *barycenter_y) {
    *barycenter_x = *barycenter_x / number_of_neighbors;
    *barycenter_y = *barycenter_y / number_of_neighbors;

    float p_x = get_x(p);
    float p_y = get_y(p);

    // if barycenter is on the particles
    if ((*barycenter_x - p_x) < 1e-6f && (*barycenter_y - p_y) < 1e-6f) {
        set_speed(p, uniform(0, 1), uniform(0, 1));
    }
    else {
        set_speed(p, p_x - *barycenter_x, p_y - *barycenter_y);
    }
}

void move_particules(env e) {
    particule p;
    
    int env_n = get_n(e);
    float env_dt = get_dt(e);
    float env_w = get_w(e);
    float env_h = get_h(e);
    float env_r = get_r(e);

    for (int i = 0; i < env_n; i++) {
        p = get_particule(e, i);
        
        move(p, env_dt);

        float new_p_x = get_x(p);
        float new_p_y = get_y(p);

        // borders collision and surpassing handling
        border_collision_handler(p, env_w, env_h, new_p_x, new_p_y);
    }

    // find neighbor particles
    for (int i = 0; i < env_n; i++) {
        p = get_particule(e, i);

        float barycenter_x = 0;
        float barycenter_y = 0;
        
        int number_of_neighbors = find_neighbors_and_barycenter(e, p, i, env_r, env_n, &barycenter_x, &barycenter_y);

        // calculate the barycenter's position and handle collision with other particles
        if (number_of_neighbors > 0) {
            particles_collision_handler(p, number_of_neighbors, &barycenter_x, &barycenter_y);
        }
    }
}