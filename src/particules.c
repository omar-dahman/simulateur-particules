/**
 * \file particules.c
 * \author Omar DAHMAN
 * \date 2026-02-20
 * \brief Individual particle management implementing particules.h.
 *
 * \note MODIFIED FOR LOT E (Tâche E.2):
 *   - struct particule_s extended with z and vz fields.
 *   - create_particule, set_position, set_speed: added z / vz parameters.
 *   - get_z, get_vz: new getter functions.
 *   - get_pos, get_vel: new vec3 convenience functions.
 *   - move: updated to move in 3D.
 *   - distanceve: updated to use 3D distance.
 */

#include <stdlib.h>
#include <math.h>
#include "particules.h"
#include "vector.h" /* ADDED FOR LOT E (Tâche E.2) */

/**
 * \internal
 * \brief Represents a particle.
 * \note MODIFIED FOR LOT E (Tâche E.2): added z and vz for 3D.
 */
struct particule_s {
    float x;  /**< particle's x-coordinate */
    float y;  /**< particle's y-coordinate */
    float z;  /**< particle's z-coordinate  [ADDED FOR LOT E - Tâche E.2] */
    float vx; /**< particle's horizontal speed */
    float vy; /**< particle's vertical speed */
    float vz; /**< particle's depth speed    [ADDED FOR LOT E - Tâche E.2] */
};

/**
 * \internal
 * \brief Normalizes a speed vector (3D) to unit length.
 * \note MODIFIED FOR LOT E (Tâche E.2): extended to 3D using vec3_normalize.
 *
 * \param vx Pointer to the x-component of the vector.
 * \param vy Pointer to the y-component of the vector.
 * \param vz Pointer to the z-component of the vector.
 */
static void normalize(float *vx, float *vy, float *vz) {
    vec3 v = vec3_make(*vx, *vy, *vz);
    vec3 n = vec3_normalize(v);
    *vx = n.x;
    *vy = n.y;
    *vz = n.z;
}


particule create_particule(float x, float y, float z, float vx, float vy, float vz) {
    particule p = malloc(sizeof(struct particule_s));
    if (p == NULL) {
        return NULL;
    }
    p->x = x;
    p->y = y;
    p->z = z;                       /* ADDED FOR LOT E (Tâche E.2) */
    normalize(&vx, &vy, &vz);
    p->vx = vx;
    p->vy = vy;
    p->vz = vz;                     /* ADDED FOR LOT E (Tâche E.2) */
    return p;
}

void free_particule(particule p) {
    free(p);
}

float get_x(particule p) {
    return p->x;
}

float get_y(particule p) {
    return p->y;
}

/* ADDED FOR LOT E (Tâche E.2) */
float get_z(particule p) {
    return p->z;
}

float get_vx(particule p) {
    return p->vx;
}

float get_vy(particule p) {
    return p->vy;
}

/* ADDED FOR LOT E (Tâche E.2) */
float get_vz(particule p) {
    return p->vz;
}

void set_position(particule p, float x, float y, float z) {
    /* MODIFIED FOR LOT E (Tâche E.2): added z */
    p->x = x;
    p->y = y;
    p->z = z;
}

void set_speed(particule p, float vx, float vy, float vz) {
    /* MODIFIED FOR LOT E (Tâche E.2): added vz */
    normalize(&vx, &vy, &vz);
    p->vx = vx;
    p->vy = vy;
    p->vz = vz;
}

void move(particule p, float dt) {
    /* MODIFIED FOR LOT E (Tâche E.2): added z-axis movement */
    p->x += p->vx * dt;
    p->y += p->vy * dt;
    p->z += p->vz * dt;
}

float distanceve(particule p1, particule p2) {
    /* MODIFIED FOR LOT E (Tâche E.2): 3D distance using vec3 library */
    vec3 pos1 = vec3_make(p1->x, p1->y, p1->z);
    vec3 pos2 = vec3_make(p2->x, p2->y, p2->z);
    return vec3_distance(pos1, pos2);
}

/* ADDED FOR LOT E (Tâche E.2) */
vec3 get_pos(particule p) {
    return vec3_make(p->x, p->y, p->z);
}

/* ADDED FOR LOT E (Tâche E.2) */
vec3 get_vel(particule p) {
    return vec3_make(p->vx, p->vy, p->vz);
}
