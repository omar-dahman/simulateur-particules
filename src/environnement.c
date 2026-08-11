/**
 * \file    environnement.c
 * \author  Omar DAHMAN
 * \date    2026-04-04
 *
 * \brief   Particles management and handling (Implementation)
 * \details This file contains implementation of functions declared in environnement.h.
 *
 * \note MODIFIED FOR LOT E (Tâche E.2):
 *   - struct env_structure: added field \a d (depth, z-axis).
 *   - create_environnement: added parameter \a d; particles now initialised with z = d/2 and vz = 0.
 *   - get_d: new getter for depth.
 *   - border_collision_handler: extended to handle z-axis borders.
 *   - find_neighbors_and_barycenter: uses 3D distance and accumulates z.
 *   - particles_collision_handler: repulsion direction now computed in 3D using vec3.
 *   - move_particules: passes depth to border handler.
 * (Tâche E.5):
 *   - struct env_structure: added fields \a objects and object_count.
 *   - create_environnement: added initialisation of parameters objects and object_count.
 *   - free_environnement: added free of objects.
 *   - add_attractor, repulsion_force, attractor_force: new function.
 *   - find_neighbors_and_barycenter, particles_collision_handler : removed due to the implementation of attraction repulsion forces.
 *   - move_particules: modified to implement attraction/repulsion forces.
 */

#include <stdlib.h>
#include <stdio.h>
#include "environnement.h"
#include "distributions.h"
#include "vector.h" /* ADDED FOR LOT E (Tâche E.2) */

/**
 * \struct env_structure
 * \brief Environment structure
 * \note MODIFIED FOR LOT E (Tâche E.2): added field \a d for depth.
 * MODIFIED FOR LOT E (Tâche E.5): added field objects and object_count.
 */
struct env_structure {
    float w;               /**< width  of the environment (x-axis) */
    float h;               /**< height of the environment (y-axis) */
    float d;               /**< depth  of the environment (z-axis) [ADDED FOR LOT E - Tâche E.2] */
    int   n;               /**< number of particles */
    float r;               /**< interaction radius */
    float dt;              /**< iteration duration */
    particule *particules; /**< array of particles */
    attractor *objects;    /**< attractors/repulsors present * ADDED FOR LOT E (Tâche E.5) */
    int object_count; /**< Number of objects (attractors/repulsors) present  ADDED FOR LOT E (Tâche E.5) */
};


env create_environnement(int n, float w, float h, float d, float r, float dt) {
    /* MODIFIED FOR LOT E (Tâche E.2): added parameter d */
    /* MODIFIED FOR LOT E (Tâche E.5): added parameters objects and object_count */
    env e = malloc(sizeof(struct env_structure));
    if (!e) {
        return NULL;
    }

    e->w  = w;
    e->h  = h;
    e->d  = d;   /* ADDED FOR LOT E (Tâche E.2) */
    e->n  = n;
    e->r  = r;
    e->dt = dt;

    e->objects = NULL; /* ADDED FOR LOT E (Tâche E.5) */
    e->object_count = 0;  /* ADDED FOR LOT E (Tâche E.5) */

    e->particules = malloc(n * sizeof(particule));
    if (!e->particules) {
        free(e);
        return NULL;
    }

    /* Place all particles at the centre of the 3D environment,
     * moving in the positive x direction.
     * MODIFIED FOR LOT E (Tâche E.2): centre now also includes d/2 for z. */
    for (int i = 0; i < n; i++) {
        e->particules[i] = create_particule(w / 2.0f, h / 2.0f, d / 2.0f,
                                             1.0f, 0.0f, 0.0f);
    }

    add_attractor(e, vec3_make(w / 2.0f, h / 2.0f, d / 2.0f), 50.0f); // attractor  /* ADDED FOR LOT E (Tâche E.5) */
    add_attractor(e, vec3_make(w / 4.0f, h / 4.0f, d / 2.0f), -80.0f);  // repulsor  /* ADDED FOR LOT E (Tâche E.5) */
    return e;
}

void free_environnement(env e) {
    /* MODIFIED FOR LOT E (Tâche E.5): free parameter objects */
    if (!e) return ;
    int env_n = get_n(e);
    for (int i = 0; i < env_n; i++) {
        free_particule(e->particules[i]);
    }
    free(e->particules);
    free(e->objects); /* ADDED FOR LOT E (Tâche E.5) */
    free(e);
}

/**
 * \internal
 * \brief Adds an attractor or repulsor to the environment.
 * \details Dynamically resizes the environment's objects array and inserts
 *          a new force source (attractor if strength > 0, repulsor if < 0)
 *          at the given 3D position. *
 * \param e         The environment.
 * \param position  3D position of the attractor/repulsor.
 * \param strength  Force intensity (positive = attractor, negative = repulsor). *
 * \note ADDED FOR LOT E (Tâche E.5): introduces external force sources acting on particles during simulation. * 
 */
void add_attractor(env e, vec3 position, float strength) {
    attractor *tmp = realloc(e->objects, (e->object_count + 1) * sizeof(attractor));
    if (!tmp) return;
    e->objects = tmp;
    e->objects[e->object_count].position = position;
    e->objects[e->object_count].strength = strength;
    e->object_count++;
}

int get_n(env e) {
    return e->n;
}

particule get_particule(env e, int i) {
    if (i < 0 || i >= e->n) {
        return NULL;
    }
    return e->particules[i];
}

float get_w(env e) { return e->w; }
float get_h(env e) { return e->h; }
float get_dt(env e) { return e->dt; }
float get_r(env e) { return e->r; }

/* ADDED FOR LOT E (Tâche E.2) */
float get_d(env e) { return e->d; }


/**
 * \brief   Border collision handler (3D)
 * \details Handles collisions of a particle with the 6 faces of the
 *          rectangular parallelepiped environment and updates position/speed.
 *
 * \param   p       particle as \a particule
 * \param   env_w   width  of the environment as \a float
 * \param   env_h   height of the environment as \a float
 * \param   env_d   depth  of the environment as \a float
 * \param   new_x   new x-coordinate after move
 * \param   new_y   new y-coordinate after move
 * \param   new_z   new z-coordinate after move [ADDED FOR LOT E - Tâche E.2]
 *
 * \note MODIFIED FOR LOT E (Tâche E.2): added z-axis handling.
 */
static void border_collision_handler(particule p,
                                     float env_w, float env_h, float env_d,
                                     float new_x, float new_y, float new_z) {
    float vx = get_vx(p);
    float vy = get_vy(p);
    float vz = get_vz(p);   /* ADDED FOR LOT E (Tâche E.2) */

    float fx = new_x;
    float fy = new_y;
    float fz = new_z;       /* ADDED FOR LOT E (Tâche E.2) */

    /* X-axis borders */
    if (new_x >= env_w) {
        fx = 2.0f * env_w - new_x;
        vx = -vx;
    } else if (new_x <= 0.0f) {
        fx = -new_x;
        vx = -vx;
    }

    /* Y-axis borders */
    if (new_y >= env_h) {
        fy = 2.0f * env_h - new_y;
        vy = -vy;
    } else if (new_y <= 0.0f) {
        fy = -new_y;
        vy = -vy;
    }

    /* Z-axis borders — ADDED FOR LOT E (Tâche E.2) */
    if (new_z >= env_d) {
        fz = 2.0f * env_d - new_z;
        vz = -vz;
    } else if (new_z <= 0.0f) {
        fz = -new_z;
        vz = -vz;
    }

    set_position(p, fx, fy, fz);
    set_speed(p, vx, vy, vz);
}

/* REMOVED FOR LOT E (Tâche E.5):
 * barycenter-based repulsion system from lot B */
// /**
//  * \brief   Find neighbor particles and compute their barycenter
//  * \details Iterates over all particles and accumulates the positions of those
//  *          within radius \a env_r of particle \a p (particle \a i is excluded).
//  *
//  * \param   e               environment
//  * \param   p               reference particle
//  * \param   i               index of reference particle (excluded from search)
//  * \param   env_r           interaction radius
//  * \param   env_n           total number of particles
//  * \param   barycenter      output: sum of neighbor positions (divided later) [MODIFIED FOR LOT E - Tâche E.2: now vec3*]
//  * \return  number of neighbors found
//  *
//  * \note MODIFIED FOR LOT E (Tâche E.2): barycenter is now a vec3 to handle 3D positions.
//  */
// static int find_neighbors_and_barycenter(env e, particule p, int i, float env_r, int env_n, vec3 *barycenter) {
//     int count = 0;
//     *barycenter = vec3_make(0.0f, 0.0f, 0.0f);

//     for (int j = 0; j < env_n; j++) {
//         if (i == j) continue;

//         particule p2 = get_particule(e, j);
//         if (distanceve(p, p2) <= env_r) {
//             *barycenter = vec3_add(*barycenter, get_pos(p2));
//             count++;
//         }
//     }
//     return count;
// }

// /**
//  * \brief   Particle repulsion handler
//  * \details Computes the barycenter of neighbors and updates the particle's
//  *          speed to move away from it. If the barycenter coincides with the
//  *          particle's position, a random direction is chosen.
//  *
//  * \param   p           particle
//  * \param   nb          number of neighbors
//  * \param   barycenter  sum of neighbor positions (not yet averaged)
//  *
//  * \note MODIFIED FOR LOT E (Tâche E.2): repulsion direction computed in 3D with vec3.
//  */
// static void particles_collision_handler(particule p, int nb, vec3 barycenter) {
//     /* Average to get the actual barycenter */
//     barycenter = vec3_scale(barycenter, 1.0f / (float)nb);

//     vec3 pos = get_pos(p);
//     vec3 away = vec3_sub(pos, barycenter);

//     if (vec3_norm(away) < 1e-6f) {
//         /* Barycenter is on the particle: choose a random direction */
//         float rx = uniform(-1.0f, 1.0f);
//         float ry = uniform(-1.0f, 1.0f);
//         float rz = uniform(-1.0f, 1.0f);
//         set_speed(p, rx, ry, rz);
//     } else {
//         /* set_speed normalizes automatically */
//         set_speed(p, away.x, away.y, away.z);
//     }
// }
/**
 * \internal
 * \brief Computes the repulsion force exerted by \a p2 on \a p1.
 * \param p1 first particle.
 * \param p2 second particle.
 * \param radius interaction radius.
 * \return  Repulsion force vector applied to p1.
 * \note ADDED FOR LOT E (Tâche E.5): replaces the barycenter-based repulsion system.
 */
static vec3 repulsion_force(particule p1, particule p2, float radius) {
    vec3 dir = vec3_sub(get_pos(p1), get_pos(p2));
    float dist = vec3_norm(dir);

    if (dist < 1e-5f || dist > radius)
        return vec3_make(0.0f, 0.0f, 0.0f);

    dir = vec3_normalize(dir);
    float strength = 1.0f / (dist * dist);
    return vec3_scale(dir, strength);
}

/**
 * \internal
 * \brief Computes force applied by an attractor/repulsor \a a on the particle \a p.
 * \param p the particle.
 * \param a the attractor.
 * \return Attractor force vector applied on \a p.
 * \note ADDED FOR LOT E (Tâche E.5): replaces the barycenter-based repulsion system.
 */
static vec3 attractor_force(particule p, attractor a) {
    vec3 dir = vec3_sub(a.position, get_pos(p));
    float dist = vec3_norm(dir);

    if (dist < 1e-5f)
        return vec3_make(0.0f, 0.0f, 0.0f);

    dir = vec3_normalize(dir);

    /* Force decreases with distance */
    float strength = a.strength / (dist * dist + 1e-3f);

    return vec3_scale(dir, strength);
}

void move_particules(env e) {
    int   env_n  = get_n(e);
    float env_dt = get_dt(e);
    float env_w  = get_w(e);
    float env_h  = get_h(e);
    float env_d  = get_d(e);   /* ADDED FOR LOT E (Tâche E.2) */
    float env_r  = get_r(e);

    /* MODIFIED FOR LOT E (Tâche E.5): Apply external forces (attractors / repulsors)*/
    /* repulsion between nearby particles */
    for (int i = 0; i < env_n; i++) {
        particule p1 = get_particule(e, i);

        // friction
        vec3 vel = get_vel(p1);
        vec3 drag = vec3_scale(vel, -0.1f);
        apply_force(p1, drag);

        // attractors
        for (int j = 0; j < e->object_count; j++) {
            vec3 f = attractor_force(p1, e->objects[j]);
            apply_force(p1, f);
        }

        // repulsion
        for (int j = i + 1; j < env_n; j++) {
            particule p2 = get_particule(e, j);
            vec3 f = repulsion_force(p1, p2, env_r);
            apply_force(p1, f);
            apply_force(p2, vec3_negate(f));
        }

        vec3 noise = vec3_make(
            uniform(-0.05f, 0.05f),
            uniform(-0.05f, 0.05f),
            uniform(-0.05f, 0.05f)
        );
        apply_force(p1, noise);
    }
    /* move every particle and handle border collisions */
    for (int i = 0; i < env_n; i++) {
        particule p = get_particule(e, i);
        move(p, env_dt);
        border_collision_handler(p, env_w, env_h, env_d, get_x(p), get_y(p), get_z(p));
        /* MODIFIED FOR LOT E (Tâche E.2): now passes get_z and env_d */
    }
}