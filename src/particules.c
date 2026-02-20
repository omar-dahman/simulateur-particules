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
 * \brief Normalize the speed vector if it's not unitary.
 */
void normalize(float *vx, float *vy) {
    /*Calculates the vector's norm.*/
    float norm = (float) sqrt((*vx)*(*vx)+(*vy)*(*vy));
    /*If it's more than 1, the vector needs to be normalized, if it's unitary nothing is to be done.*/
    /*"1e-f" is used instead of 1 to avoid floating precision errors.*/
    if(norm > 1e-6f) {
        *vx/=norm;
        *vy/=norm;
    }
}

particule create_particule(float x, float y, float vx, float vy) {
    particule p = malloc(sizeof(struct particule_s));
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
    float distance = (float) sqrt(pow((p2->x) - (p1->x), 2) + pow((p2->y) - (p1->y), 2));
    return distance;
}