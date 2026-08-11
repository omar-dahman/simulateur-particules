/**
 * \file    test_forces.c
 * \author  Othmane CHAOUI
 * \date    2026-05-11
 *
 * \brief   Tests of acceleration, gravity and repulsion
 * \details This file contains the tests on changes made
 *          by \b Task \b E.5
 */

#include <math.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "particules.h"
#include "environnement.h"
#include "vector.h"

#define EPSILON 1e-4f

/**
 * \brief   Test: Repulsion between two particles
 * \details Creates two particles close to each other (distance < r),
 *          applies multiple simulation steps, and verifies they move apart.
 *          Uses create_environnement() but then repositions particles
 *          and relies on repulsion force overcoming default attractors.
 */
void test_repulsion_two_particles()
{
    /* Create environment with 2 particles and default dt = 0.1 */
    env e = create_environnement(2, 10.0f, 10.0f, 10.0f, 2.0f, 0.1f);
    CU_ASSERT_PTR_NOT_NULL_FATAL(e);

    /* Get particles */
    particule p1 = get_particule(e, 0);
    particule p2 = get_particule(e, 1);

    /* Place them close to each other (distance < r = 2.0) */
    set_position(p1, 0.0f, 0.0f, 0.0f);
    set_position(p2, 0.8f, 0.0f, 0.0f);

    /* Set zero initial speeds */
    set_speed(p1, 0.0f, 0.0f, 0.0f);
    set_speed(p2, 0.0f, 0.0f, 0.0f);

    /* Reset forces/accelerations by reinitializing particle state */
    /* (apply_force accumulates acceleration, we need fresh state) */
    /* Instead, we can simply simulate and check if distance increases */

    /* Save initial distance */
    float dist_initial = distanceve(p1, p2);

    /* Run multiple simulation steps */
    for (int step = 0; step < 20; step++)
    {
        move_particules(e);
    }

    /* Verify particles moved apart */
    float dist_final = distanceve(p1, p2);
    CU_ASSERT_TRUE(dist_final > dist_initial + EPSILON);

    free_environnement(e);
}

/**
 * \brief   Test: Attraction between a particle and an attractor
 * \details Creates a particle, adds a strong attractor, and verifies
 *          the particle moves toward it.
 */
void test_attraction_particle_attractor()
{
    /* Create environment with 1 particle */
    env e = create_environnement(1, 20.0f, 20.0f, 20.0f, 2.0f, 0.1f);
    CU_ASSERT_PTR_NOT_NULL_FATAL(e);

    /* Get particle */
    particule p = get_particule(e, 0);

    /* Place particle at origin */
    set_position(p, 0.0f, 0.0f, 0.0f);
    set_speed(p, 0.0f, 0.0f, 0.0f);

    /* Add a strong attractor to the right (positive X) */
    /* Note: create_environnement already adds default attractors,
       but we can add a stronger one to dominate the movement */
    add_attractor(e, vec3_make(8.0f, 0.0f, 0.0f), 500.0f);

    /* Save initial position */
    vec3 pos_initial = get_pos(p);

    /* Run multiple simulation steps */
    for (int step = 0; step < 30; step++)
    {
        move_particules(e);
    }

    /* Verify particle moved (attracted) */
    vec3 pos_final = get_pos(p);
    float distance_moved = vec3_distance(pos_initial, pos_final);

    CU_ASSERT_TRUE(distance_moved > EPSILON);
    /* Check movement is toward positive X (the attractor direction) */
    CU_ASSERT_TRUE(pos_final.x > pos_initial.x + EPSILON);

    free_environnement(e);
}

/**
 * \brief   Registers all force tests with CUnit
 */
CU_ErrorCode test_forces_register_suite()
{
    CU_pSuite suite = CU_add_suite("Forces (E.5)", NULL, NULL);
    if (!suite)
        return CU_get_error();

    CU_add_test(suite, "test_repulsion_two_particles", test_repulsion_two_particles);
    CU_add_test(suite, "test_attraction_particle_attractor", test_attraction_particle_attractor);

    return CUE_SUCCESS;
}