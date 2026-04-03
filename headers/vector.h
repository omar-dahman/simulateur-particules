/**
 * \file    vector.h
 * \author Omar DAHMAN
 * \brief   3D vector library
 * \details This file declares a complete 3D vector library for use in the
 *          particle simulation. It provides types and functions for vector
 *          arithmetic, geometry and transformations in 3D space.
 *
 * \note    ADDED FOR LOT E (Tâche E.2) : entire file is new.
 */

#ifndef VECTOR_H
#define VECTOR_H

/**
 * \typedef vec3
 * \brief   A 3D vector (or point) with floating-point coordinates.
 */
typedef struct vec3_s {
    float x; /**< x-component */
    float y; /**< y-component */
    float z; /**< z-component */
} vec3;

/**
 * \brief   Build a vec3 from three floats.
 * \param   x   x-component
 * \param   y   y-component
 * \param   z   z-component
 * \return  The new vector.
 */
vec3 vec3_make(float x, float y, float z);

/**
 * \brief   Return a copy of a vector.
 * \param   v   Source vector.
 * \return  A copy of \a v.
 */
vec3 vec3_copy(vec3 v);

/**
 * \brief   Add two vectors.
 * \param   a   First vector.
 * \param   b   Second vector.
 * \return  a + b
 */
vec3 vec3_add(vec3 a, vec3 b);

/**
 * \brief   Subtract two vectors.
 * \param   a   First vector.
 * \param   b   Second vector.
 * \return  a - b
 */
vec3 vec3_sub(vec3 a, vec3 b);

/**
 * \brief   Multiply a vector by a scalar.
 * \param   v       The vector.
 * \param   scalar  The scalar.
 * \return  v * scalar
 */
vec3 vec3_scale(vec3 v, float scalar);

/**
 * \brief   Negate a vector.
 * \param   v   The vector.
 * \return  -v
 */
vec3 vec3_negate(vec3 v);


/**
 * \brief   Compute the Euclidean norm (length) of a vector.
 * \param   v   The vector.
 * \return  ||v||
 */
float vec3_norm(vec3 v);

/**
 * \brief   Compute the squared norm of a vector (faster, avoids sqrt).
 * \param   v   The vector.
 * \return  ||v||²
 */
float vec3_norm2(vec3 v);

/**
 * \brief   Return the unit vector in the same direction as \a v.
 * \details If \a v is the zero vector, the zero vector is returned.
 * \param   v   The vector to normalize.
 * \return  v / ||v||  or  (0,0,0) if ||v|| < 1e-6
 */
vec3 vec3_normalize(vec3 v);



/**
 * \brief   Compute the dot product of two vectors.
 * \param   a   First vector.
 * \param   b   Second vector.
 * \return  a · b
 */
float vec3_dot(vec3 a, vec3 b);

/**
 * \brief   Compute the cross product of two vectors.
 * \param   a   First vector.
 * \param   b   Second vector.
 * \return  a × b  (vector orthogonal to both a and b)
 */
vec3 vec3_cross(vec3 a, vec3 b);


/**
 * \brief   Compute the angle (in radians) between two vectors.
 * \details Uses the arc-cosine of the normalized dot product.
 *          Returns 0 if either vector is zero.
 * \param   a   First vector.
 * \param   b   Second vector.
 * \return  Angle in radians in [0, π].
 */
float vec3_angle(vec3 a, vec3 b);

/**
 * \brief   Compute the Euclidean distance between two points.
 * \param   a   First point.
 * \param   b   Second point.
 * \return  ||a - b||
 */
float vec3_distance(vec3 a, vec3 b);


/**
 * \brief   Project vector \a v onto vector \a onto.
 * \details Returns the component of \a v in the direction of \a onto.
 *          Returns the zero vector if \a onto is zero.
 * \param   v       Vector to project.
 * \param   onto    Direction to project onto.
 * \return  (v · onto / ||onto||²) * onto
 */
vec3 vec3_project(vec3 v, vec3 onto);

/**
 * \brief   Reflect vector \a v about a surface with unit normal \a n.
 * \details Computes v - 2*(v·n)*n.
 * \param   v   Incident vector.
 * \param   n   Unit normal of the reflection surface.
 * \return  Reflected vector.
 */
vec3 vec3_reflect(vec3 v, vec3 n);


/**
 * \brief   Rotate vector \a v around the X axis by \a angle radians.
 * \param   v       The vector to rotate.
 * \param   angle   Rotation angle in radians.
 * \return  Rotated vector.
 */
vec3 vec3_rotate_x(vec3 v, float angle);

/**
 * \brief   Rotate vector \a v around the Y axis by \a angle radians.
 * \param   v       The vector to rotate.
 * \param   angle   Rotation angle in radians.
 * \return  Rotated vector.
 */
vec3 vec3_rotate_y(vec3 v, float angle);

/**
 * \brief   Rotate vector \a v around the Z axis by \a angle radians.
 * \param   v       The vector to rotate.
 * \param   angle   Rotation angle in radians.
 * \return  Rotated vector.
 */
vec3 vec3_rotate_z(vec3 v, float angle);

/**
 * \brief   Rotate vector \a v around an arbitrary unit axis by \a angle radians.
 * \details Uses Rodrigues' rotation formula:
 *          v_rot = v*cos(a) + (axis×v)*sin(a) + axis*(axis·v)*(1-cos(a))
 * \param   v       The vector to rotate.
 * \param   axis    Unit rotation axis (should be normalized).
 * \param   angle   Rotation angle in radians.
 * \return  Rotated vector.
 */
vec3 vec3_rotate(vec3 v, vec3 axis, float angle);



/**
 * \brief   Linear interpolation between two vectors.
 * \param   a   Start vector (t = 0).
 * \param   b   End vector   (t = 1).
 * \param   t   Interpolation parameter (clamped to [0, 1]).
 * \return  a + t * (b - a)
 */
vec3 vec3_lerp(vec3 a, vec3 b, float t);

/**
 * \brief   Test whether two vectors are approximately equal.
 * \param   a       First vector.
 * \param   b       Second vector.
 * \param   eps     Tolerance.
 * \return  1 if ||a - b|| < eps, 0 otherwise.
 */
int vec3_equal(vec3 a, vec3 b, float eps);

#endif /* VECTOR_H */
