/**
 * \file    environnement.h
 * \author  Omar DAHMAN
 * \date    2026-04-04
 *
 * \brief   Particles management and handling
 * \details This file contains declarations for functions that will
 *          allow managing the set of particles, their movements,
 *          their interactions and the interactions between the
 *          particles and the edges.
 *
 * \note MODIFIED FOR LOT E (Tâche E.2):
 *   - create_environnement: added parameter \a d for depth (3D).
 *   - get_d: new function to retrieve the depth of the environment.
 */

#ifndef _ENVIRONNEMENT_H
#define _ENVIRONNEMENT_H

#include "particules.h" /* for the type "particule" in get_particule */
#include "vector.h" /* for the type "vec3" in attractor [ADDED FOR LOT E (Tâche E.5)] */ 

/**
 * \typedef \a env
 * \brief   Type for an \b environment
 * \details This type is used for variables representing an environment of particles.
 */
typedef struct env_structure* env;

/**
 * \struct attractor_s
 * \brief Object that applies a force on particles (attractor or repulsor) *
 * \note ADDED FOR LOT E (Tâche E.5)
 */
typedef struct attractor_s {
    vec3 position;  /**< position in 3D space */
    float strength; /**< >0 attractor, <0 repulsor */
} attractor;

/**
 * \brief Types of obstacles that can be placed in the environment
 */
typedef enum
{
    OBSTACLE_PLANE,  /**< Infinite plane (or half-space) */
    OBSTACLE_SPHERE, /**< Sphere with a given radius */
    OBSTACLE_BOX     /**< Axis-aligned box (parallelepiped) */
} ObstacleType;

/**
 * \brief Structure representing a static obstacle in the environment
 * \details Particles will collide with and bounce off these obstacles.
 *          The restitution coefficient determines how bouncy the obstacle is:
 *          - 0.0f : fully inelastic (particle sticks)
 *          - 1.0f : perfectly elastic (full bounce)
 */
typedef struct {
    ObstacleType type; /**< Type of obstacle (plane, sphere, box) */
    vec3 position;     /**< Center (sphere/box) or a point on the plane */
    vec3 normal;       /**< Unit normal vector (for plane obstacle only) */
    vec3 half_size;    /**< Half-extents in x,y,z (for box obstacle only) */
    float radius;      /**< Radius (for sphere obstacle only) */
    float restitution; /**< Bounciness coefficient [0..1] */
} Obstacle;

/**
 * \brief Returns the number of obstacles in the environment
 * \param e Environment pointer
 * \return Number of obstacles
 */
int get_obstacle_count(env e);

/**
 * \brief Returns a pointer to the obstacle at the given index
 * \param e Environment pointer
 * \param index Index of the obstacle (0 to get_obstacle_count()-1)
 * \return Pointer to the obstacle, or NULL if index is invalid
 */
const Obstacle *get_obstacle(env e, int index);

/**
 * \brief Adds a static obstacle to the environment
 * \param e        Environment pointer
 * \param obstacle The obstacle to add (will be copied)
 */
void add_obstacle(env e, Obstacle obstacle);

/**
 * \brief   Creates an environment of particles
 * \details Performs a memory allocation for an environment of size w x h x d
 *          for n particles. The particles are all placed at the center of the
 *          environment with a speed that sends them in the positive x direction.
 *
 * \param   n   number of particles as \a integer
 * \param   w   width  of the environment (x-axis) as \a float
 * \param   h   height of the environment (y-axis) as \a float
 * \param   d   depth  of the environment (z-axis) as \a float  [ADDED FOR LOT E - Tâche E.2]
 * \param   r   interaction radius as \a float
 * \param   dt  iteration duration as \a float
 * \return  environment of particles as \a env
 *
 * \note Signature MODIFIED FOR LOT E (Tâche E.2): added parameter \a d for depth.
 */
env create_environnement(int n, float w, float h, float d, float r, float dt);

/**
 * \brief    Frees environment memory
 * \details  Frees the memory allocated for the environment and all its particles.
 *
 * \param   e   environment as \a env
 */
void free_environnement(env e);

/**
 * \brief Adds an attractor or repulsor to the environment.
 * \param e environment
 * \param position position in 3D space
 * \param strength force strength (>0 attractor, <0 repulsor)
 */
void add_attractor(env e, vec3 position, float strength);

/**
 * \brief   Returns number of particles
 * \param   e   environment as \a env
 * \return  number of particles as \a integer
 */
int get_n(env e);

/**
 * \brief   Returns a particle
 * \param   e   environment as \a env
 * \param   i   index of the particle as \a integer
 * \return  i-th particle of the environment as \a particule
 */
particule get_particule(env e, int i);

/**
 * \brief   Returns width of environment (x-axis)
 * \param   e   environment as \a env
 * \return  Width as \a float
 */
float get_w(env e);

/**
 * \brief   Returns height of environment (y-axis)
 * \param   e   environment as \a env
 * \return  Height as \a float
 */
float get_h(env e);

/**
 * \brief   Returns depth of environment (z-axis)
 * \param   e   environment as \a env
 * \return  Depth as \a float
 * \note ADDED FOR LOT E (Tâche E.2) : 3D extension.
 */
float get_d(env e);

/**
 * \brief   Returns duration of iterations
 * \param   e   environment as \a env
 * \return  dt as \a float
 */
float get_dt(env e);

/**
 * \brief   Returns interaction radius
 * \param   e   environment as \a env
 * \return  r as \a float
 */
float get_r(env e);

/**
 * \brief   Performs one simulation iteration on all particles.
 * \details Computes interaction forces between particles,
 *          applies attractor/repulsor forces and fluid friction,
 *          updates particle accelerations and velocities,
 *          then moves particles and handles 3D border collisions.
 * \param   e   environment as \a env *
 * \note MODIFIED FOR LOT E (Tâche E.5):
 *       particle interactions now use forces and acceleration
 *       instead of direct velocity modification.
 */
void move_particules(env e);

#endif 
