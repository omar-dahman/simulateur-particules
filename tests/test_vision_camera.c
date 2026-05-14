/**
 * \file    test_vision_camera.c
 * \author  Othmane CHAOUI
 * \date    2026-05-14
 *
 * \brief   Tests of camera's vision
 * \details This file contains the tests on changes made
 *          by \b Task \b E.4
 */

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "camera.h"
#include "vector.h"

#define EPSILON 1e-5f

/**
 * \brief   Test: Particle directly in front of the camera is visible
 * \details Verifies that a particle placed along the camera's gaze direction
 *          is detected as visible and projected within the screen bounds.
 */
void test_visible_front()
{
    vec3 cam_pos = {0, 0, 0};
    vec3 cam_dir = {1, 0, 0};
    Camera cam = camera_create(cam_pos, cam_dir, 100, 100);

    vec3 particle = {10, 0, 0};
    int px, py;
    int visible = camera_is_particle_visible(&cam, particle, &px, &py);

    CU_ASSERT_TRUE(visible);
    CU_ASSERT_TRUE(px >= 0 && px < 100);
    CU_ASSERT_TRUE(py >= 0 && py < 100);
}

/**
 * \brief   Test: Particle behind the camera is not visible
 * \details Verifies that a particle located behind the camera
 *          (negative direction along the gaze axis) is not visible.
 */
void test_visible_behind()
{
    vec3 cam_pos = {0, 0, 0};
    vec3 cam_dir = {1, 0, 0};
    Camera cam = camera_create(cam_pos, cam_dir, 100, 100);

    vec3 particle = {-10, 0, 0};
    int px, py;
    int visible = camera_is_particle_visible(&cam, particle, &px, &py);

    CU_ASSERT_FALSE(visible);
}

/**
 * \brief   Test: Particle out of vertical field of view
 * \details Verifies that a particle placed in front of the camera but
 *          with a very high Y coordinate is outside the vertical
 *          field of view and thus not visible.
 */
void test_visible_out_of_field_vertical()
{
    vec3 cam_pos = {0, 0, 0};
    vec3 cam_dir = {1, 0, 0};
    Camera cam = camera_create(cam_pos, cam_dir, 100, 100);

    // Particle in front but very high → out of vertical field of view
    vec3 particle = {10, 1000, 0};
    int px, py;
    int visible = camera_is_particle_visible(&cam, particle, &px, &py);

    CU_ASSERT_FALSE(visible);
}

/**
 * \brief   Test: Particle out of horizontal field of view
 * \details Verifies that a particle placed in front of the camera but
 *          with a very large Z coordinate is outside the horizontal
 *          field of view and thus not visible.
 */
void test_visible_out_of_field_horizontal()
{
    vec3 cam_pos = {0, 0, 0};
    vec3 cam_dir = {1, 0, 0};
    Camera cam = camera_create(cam_pos, cam_dir, 100, 100);

    // Particle in front but very far to the right → out of horizontal field of view
    vec3 particle = {10, 0, 1000};
    int px, py;
    int visible = camera_is_particle_visible(&cam, particle, &px, &py);

    CU_ASSERT_FALSE(visible);
}

/**
 * \brief   Test: Particle projects to the center of the screen
 * \details Verifies that a particle placed exactly along the camera's
 *          gaze direction projects to the center pixel of the screen.
 *          For a 100x100 screen, the center is at (50, 50).
 */
void test_projection_center()
{
    vec3 cam_pos = {0, 0, 0};
    vec3 cam_dir = {1, 0, 0};
    Camera cam = camera_create(cam_pos, cam_dir, 100, 100);

    vec3 particle = {10, 0, 0};
    int px, py;
    camera_is_particle_visible(&cam, particle, &px, &py);

    // The center of a 100x100 screen is at (50, 50)
    CU_ASSERT_EQUAL(px, 50);
    CU_ASSERT_EQUAL(py, 50);
}

/**
 * \brief   Registers all camera vision tests with CUnit
 * \details Creates a test suite named "Vision Camera (E.4)" and adds
 *          all test functions to it. This function is called from
 *          tests_main.c to include these tests in the test run.
 *
 * \return  CUE_SUCCESS if suite creation succeeded,
 *          error code otherwise.
 */
CU_ErrorCode test_vision_camera_register_suite()
{
    CU_pSuite suite = CU_add_suite("Vision Camera (E.4)", NULL, NULL);
    if (!suite)
        return CU_get_error();

    CU_add_test(suite, "visible_front", test_visible_front);
    CU_add_test(suite, "visible_behind", test_visible_behind);
    CU_add_test(suite, "visible_out_of_field_vertical", test_visible_out_of_field_vertical);
    CU_add_test(suite, "visible_out_of_field_horizontal", test_visible_out_of_field_horizontal);
    CU_add_test(suite, "projection_center", test_projection_center);

    return CUE_SUCCESS;
}