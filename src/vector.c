/**
 * \file    vector.c
 * \author Omar DAHMAN
 * \brief   3D vector library implementation
 * \details Implements all functions declared in vector.h.
 *
 * \note    ADDED FOR LOT E (Tâche E.2) : entire file is new.
 */

#include <math.h>
#include "vector.h"


vec3 vec3_make(float x, float y, float z) {
    vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

vec3 vec3_copy(vec3 v) {
    return vec3_make(v.x, v.y, v.z);
}


vec3 vec3_add(vec3 a, vec3 b) {
    return vec3_make(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 vec3_sub(vec3 a, vec3 b) {
    return vec3_make(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3 vec3_scale(vec3 v, float scalar) {
    return vec3_make(v.x * scalar, v.y * scalar, v.z * scalar);
}

vec3 vec3_negate(vec3 v) {
    return vec3_make(-v.x, -v.y, -v.z);
}


float vec3_norm(vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float vec3_norm2(vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

vec3 vec3_normalize(vec3 v) {
    float n = vec3_norm(v);
    if (n < 1e-6f) {
        return vec3_make(0.0f, 0.0f, 0.0f);
    }
    return vec3_scale(v, 1.0f / n);
}


float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 vec3_cross(vec3 a, vec3 b) {
    return vec3_make(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}


float vec3_angle(vec3 a, vec3 b) {
    float na = vec3_norm(a);
    float nb = vec3_norm(b);

    /* Avoid division by zero */
    if (na < 1e-6f || nb < 1e-6f) {
        return 0.0f;
    }

    /* Clamp dot product to [-1, 1] to avoid NaN from acosf */
    float cos_angle = vec3_dot(a, b) / (na * nb);
    if (cos_angle >  1.0f) cos_angle =  1.0f;
    if (cos_angle < -1.0f) cos_angle = -1.0f;

    return acosf(cos_angle);
}

float vec3_distance(vec3 a, vec3 b) {
    return vec3_norm(vec3_sub(a, b));
}


vec3 vec3_project(vec3 v, vec3 onto) {
    float onto_norm2 = vec3_norm2(onto);

    /* Avoid division by zero */
    if (onto_norm2 < 1e-12f) {
        return vec3_make(0.0f, 0.0f, 0.0f);
    }

    float scalar = vec3_dot(v, onto) / onto_norm2;
    return vec3_scale(onto, scalar);
}

vec3 vec3_reflect(vec3 v, vec3 n) {
    /* v - 2 * (v·n) * n  (n is assumed to be a unit vector) */
    float dot = vec3_dot(v, n);
    return vec3_sub(v, vec3_scale(n, 2.0f * dot));
}

vec3 vec3_rotate_x(vec3 v, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    return vec3_make(
        v.x,
        v.y * c - v.z * s,
        v.y * s + v.z * c
    );
}

vec3 vec3_rotate_y(vec3 v, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    return vec3_make(
        v.x * c + v.z * s,
        v.y,
       -v.x * s + v.z * c
    );
}

vec3 vec3_rotate_z(vec3 v, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    return vec3_make(
        v.x * c - v.y * s,
        v.x * s + v.y * c,
        v.z
    );
}

vec3 vec3_rotate(vec3 v, vec3 axis, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);

    vec3 term1 = vec3_scale(v, c);
    vec3 term2 = vec3_scale(vec3_cross(axis, v), s);
    vec3 term3 = vec3_scale(axis, vec3_dot(axis, v) * (1.0f - c));

    return vec3_add(vec3_add(term1, term2), term3);
}

vec3 vec3_lerp(vec3 a, vec3 b, float t) {
    /* Clamp t to [0, 1] */
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return vec3_add(a, vec3_scale(vec3_sub(b, a), t));
}

int vec3_equal(vec3 a, vec3 b, float eps) {
    return vec3_distance(a, b) < eps;
}
