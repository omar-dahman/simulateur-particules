#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "particules.h"

/**
 * \file particules.c
 * \author Nell TELECHEA
 * \date 2026-02-20
 * \brief Individual particle management implementing particules.h.
 */


/**
 * \internal
 * \brief Represents a particle.
 */
struct particule_s
{
    float x; /**< particle's x-coordonate */
    float y; /**< particle's y-coordonate */
    float vx; /**< particle's horizontal speed */
    float vy; /**< particle's vertical speed */
};

/**
 * \internal
 * \brief Normalizes a speed vector to unit length.
 *
 * \param vx Pointer to the x-component of the vector.
 * \param vy Pointer to the y-component of the vector.
 */
void normalize(float *vx, float *vy) {
    /*Calculates the vector's norm.*/
    float norm = (float) sqrt((*vx)*(*vx)+(*vy)*(*vy));
    /*Normalize only if the norm is significantly greater than zero (to avoid division by zero and floating-point precision issues).*/
    if(norm > 1e-6f) {
        *vx/=norm;
        *vy/=norm;
    }
}

particule create_particule(float x, float y, float vx, float vy) {
    particule p = malloc(sizeof(struct particule_s));
    if(p==NULL){
        return NULL;
    }
    p->x = x;
    p->y = y;
    normalize(&vx,&vy);
    p->vx = vx;
    p->vy = vy;
    return p;
}

void free_particule(particule p){
    free(p);
}

float get_x(particule p){
    return p->x;
}

float get_y(particule p){
    return p->y;
}

float get_vx(particule p){
    return p->vx;
}

float get_vy(particule p){
    return p->vy;
}

void set_position(particule p, float x, float y){
    p->x=x;
    p->y=y;
}

void set_speed(particule p, float vx, float vy){
    normalize(&vx,&vy);
    p->vx=vx;
    p->vy=vy;
}

void move(particule p, float dt){
    p->x=p->x+(p->vx*dt);
    p->y=p->y+(p->vy*dt);
}

float distanceve(particule p1, particule p2){
    float distance = (float) sqrt(((p2->x) - (p1->x))*((p2->x) - (p1->x)) + ((p2->y) - (p1->y))*((p2->y) - (p1->y)));
    return distance;
}