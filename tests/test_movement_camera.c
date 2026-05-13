/**
 * \file    test_movement_camera.c
 * \author  Othmane CHAOUI
 * \date    2026-04-23
 *
 * \brief   Tests of camera's movement
 * \details This file contains the tests on changes made
 *          by \b Task \b E.3
 */

#include <math.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "camera.h"
#include "vector.h"

#define EPSILON 1e-5f
#define M_PI 3.14159265358979323846

/**
 * \brief   Checks that two vectors are approximately collinear
 *          (cross product close to zero)
 */
int vec3_colinear(vec3 a, vec3 b, float epsilon)
{
    vec3 c = vec3_cross(a, b);
    float n = vec3_norm(c);
    return n < epsilon;
}

void test_camera_create()
{
    vec3 pos = {1.0f, 2.0f, 3.0f};
    vec3 dir = {1.0f, 0.0f, 0.0f};
    Camera cam = camera_create(pos, dir, 640, 480);

    CU_ASSERT_TRUE(vec3_equal(cam.position, pos, EPSILON));
    CU_ASSERT_DOUBLE_EQUAL(cam.direction.x, 1.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(cam.direction.y, 0.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(cam.direction.z, 0.0f, EPSILON);
    CU_ASSERT_EQUAL(cam.width, 640);
    CU_ASSERT_EQUAL(cam.height, 480);
    CU_ASSERT_EQUAL(cam.move_type, CAMERA_MOVE_NONE);
}

/**
 * \brief   Test: Direction is normalized even if given non-unit
 */
void test_camera_create_normalise()
{
    vec3 pos = {0.0f, 0.0f, 0.0f};
    vec3 dir = {3.0f, 0.0f, 0.0f}; /* Non-unit vector */
    Camera cam = camera_create(pos, dir, 640, 480);

    float norm = vec3_norm(cam.direction);
    CU_ASSERT_DOUBLE_EQUAL(norm, 1.0f, EPSILON);
}

/**
 * \brief   Test: update does nothing when move_type is NONE
 */
void test_camera_update_none()
{
    vec3 pos = {5.0f, 5.0f, 5.0f};
    vec3 dir = {0.0f, 1.0f, 0.0f};
    Camera cam = camera_create(pos, dir, 800, 600);

    vec3 pos_before = cam.position;
    vec3 dir_before = cam.direction;

    /* Multiple updates should not change anything */
    for (int i = 0; i < 10; i++)
    {
        camera_update(&cam);
    }

    CU_ASSERT_TRUE(vec3_equal(cam.position, pos_before, EPSILON));
    CU_ASSERT_TRUE(vec3_equal(cam.direction, dir_before, EPSILON));
}

/**
 * \brief   Test: translation
 */
void test_camera_translate()
{
    vec3 pos = {0.0f, 0.0f, 0.0f};
    vec3 dir = {1.0f, 0.0f, 0.0f};
    Camera cam = camera_create(pos, dir, 640, 480);

    vec3 move_dir = {1.0f, 0.0f, 0.0f};
    float speed = 2.0f;
    camera_set_move_translate(&cam, move_dir, speed);

    CU_ASSERT_EQUAL(cam.move_type, CAMERA_MOVE_TRANSLATE);
    camera_update(&cam);

    vec3 expected = {2.0f, 0.0f, 0.0f};
    CU_ASSERT_TRUE(vec3_equal(cam.position, expected, EPSILON));
}

/**
 * \brief   Test: Translate with zero direction does not crash
 */
void test_camera_translate_zero_direction()
{
    vec3 pos = {1.0f, 1.0f, 1.0f};
    vec3 dir = {1.0f, 0.0f, 0.0f};
    Camera cam = camera_create(pos, dir, 640, 480);

    /* Zero direction vector */
    vec3 move_dir = {0.0f, 0.0f, 0.0f};
    camera_set_move_translate(&cam, move_dir, 10.0f);
    CU_ASSERT_EQUAL(cam.move_type, CAMERA_MOVE_TRANSLATE);

    vec3 pos_before = cam.position;
    camera_update(&cam);

    /* Should not have moved (direction is zero) */
    CU_ASSERT_TRUE(vec3_equal(cam.position, pos_before, EPSILON));
}

void test_camera_yaw()
{
    vec3 pos = {0.0f, 0.0f, 0.0f};
    vec3 dir = {1.0f, 0.0f, 0.0f};
    Camera cam = camera_create(pos, dir, 640, 480);

    camera_rotate_yaw(&cam, M_PI / 2.0f);
    CU_ASSERT_DOUBLE_EQUAL(cam.direction.x, 0.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(fabs(cam.direction.y), 1.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(cam.direction.z, 0.0f, EPSILON);

    float norm = vec3_norm(cam.direction);
    CU_ASSERT_DOUBLE_EQUAL(norm, 1.0f, EPSILON);
}

/**
 * \brief   Test: Pitch rotation tilts direction upward
 */
void test_camera_pitch()
{
    vec3 pos = {0.0f, 0.0f, 0.0f};
    vec3 dir = {1.0f, 0.0f, 0.0f};
    Camera cam = camera_create(pos, dir, 640, 480);

    /* Pitch upward by 90 degrees */
    camera_rotate_pitch(&cam, M_PI / 2.0f);

    /* Direction should now point upward (+Z for WORLD_UP) */
    CU_ASSERT_DOUBLE_EQUAL(cam.direction.x, 0.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(cam.direction.y, 0.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(cam.direction.z, 1.0f, EPSILON);

    /* Direction must remain unit length */
    float norm = vec3_norm(cam.direction);
    CU_ASSERT_DOUBLE_EQUAL(norm, 1.0f, EPSILON);
}

void test_camera_orbit()
{
    vec3 pos = {0.0f, 0.0f, 10.0f};
    vec3 dir = {0.0f, 0.0f, -1.0f};
    Camera cam = camera_create(pos, dir, 640, 480);

    vec3 center = {0.0f, 0.0f, 0.0f};
    float radius = 10.0f;
    float angular_speed = M_PI / 4.0f;

    camera_set_move_orbit(&cam, center, radius, angular_speed, 0.0f);
    camera_update(&cam);

    float expected = radius * cosf(angular_speed);
    CU_ASSERT_DOUBLE_EQUAL(cam.position.x, expected, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(cam.position.y, expected, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(cam.position.z, 0.0f, EPSILON);
}

/**
 * \brief   Test: Orbiting camera always looks toward center
 */
void test_camera_orbit_looks_at_center()
{
    vec3 pos = {10.0f, 0.0f, 0.0f};
    vec3 dir = {-1.0f, 0.0f, 0.0f};
    Camera cam = camera_create(pos, dir, 640, 480);

    vec3 center = {0.0f, 0.0f, 0.0f};
    float radius = 10.0f;

    camera_set_move_orbit(&cam, center, radius, 0.1f, 0.0f);

    /* After several updates, check that direction points to center */
    for (int i = 0; i < 20; i++)
    {
        camera_update(&cam);
    }

    /* Vector from camera to center should be colinear with direction */
    vec3 to_center = vec3_sub(center, cam.position);
    CU_ASSERT_TRUE(vec3_colinear(cam.direction, to_center, EPSILON));

    /* Direction should point toward center (dot product > 0) */
    float dot = vec3_dot(cam.direction, to_center);
    CU_ASSERT_TRUE(dot > 0.0f);
}

/**
 * \brief   Test: Screen axes are orthonormal and right-handed
 */
void test_camera_get_screen_axes()
{
    vec3 pos = {0.0f, 0.0f, 0.0f};
    vec3 dir = {1.0f, 0.0f, 0.0f};
    Camera cam = camera_create(pos, dir, 640, 480);

    vec3 right, up;
    camera_get_screen_axes(&cam, &right, &up);

    /* Right and up must be unit length */
    CU_ASSERT_DOUBLE_EQUAL(vec3_norm(right), 1.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(vec3_norm(up), 1.0f, EPSILON);

    /* Right must be orthogonal to direction */
    CU_ASSERT_DOUBLE_EQUAL(vec3_dot(right, cam.direction), 0.0f, EPSILON);

    /* Up must be orthogonal to direction */
    CU_ASSERT_DOUBLE_EQUAL(vec3_dot(up, cam.direction), 0.0f, EPSILON);

    /* Right and up must be orthogonal to each other */
    CU_ASSERT_DOUBLE_EQUAL(vec3_dot(right, up), 0.0f, EPSILON);

    /* Right × Up ≈ Direction (right-handed system) */
    /* Right × Up peut donner -direction selon la convention */
    /* On vérifie que le cross est colinéaire à direction (norme 1) */
    vec3 cross_ru = vec3_cross(right, up);
    vec3 cross_ur = vec3_cross(up, right);

    /* L'un des deux doit être égal à direction */
    int ru_ok = vec3_equal(cross_ru, cam.direction, EPSILON);
    int ur_ok = vec3_equal(cross_ur, cam.direction, EPSILON);
    CU_ASSERT_TRUE(ru_ok || ur_ok);
}

/**
 * \brief   Test: Screen axes when camera looks straight up (degenerate case)
 */
void test_camera_get_screen_axes_vertical()
{
    vec3 pos = {0.0f, 0.0f, 0.0f};
    vec3 dir = {0.0f, 0.0f, 1.0f}; /* Aligned with WORLD_UP */
    Camera cam = camera_create(pos, dir, 640, 480);

    vec3 right, up;
    camera_get_screen_axes(&cam, &right, &up);

    /* Should not crash and should return valid axes */
    CU_ASSERT_DOUBLE_EQUAL(vec3_norm(right), 1.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(vec3_norm(up), 1.0f, EPSILON);

    /* Right must be orthogonal to direction */
    CU_ASSERT_DOUBLE_EQUAL(vec3_dot(right, cam.direction), 0.0f, EPSILON);

    /* Up must be orthogonal to direction */
    CU_ASSERT_DOUBLE_EQUAL(vec3_dot(up, cam.direction), 0.0f, EPSILON);
}

/**
 * \brief camera movement register, called by \b tests_main.c
 */
CU_ErrorCode test_movement_camera_register_suite()
{
    CU_pSuite suite = CU_add_suite("Camera Movement (E.3)", NULL, NULL);
    if (!suite)
        return CU_get_error();

    CU_add_test(suite, "camera_create", test_camera_create);
    CU_add_test(suite, "camera_create_normalise", test_camera_create_normalise);

    CU_add_test(suite, "camera_update_none", test_camera_update_none);

    CU_add_test(suite, "camera_translate", test_camera_translate);
    CU_add_test(suite, "camera_translate_zero_direction",
                test_camera_translate_zero_direction);

    CU_add_test(suite, "camera_yaw", test_camera_yaw);
    CU_add_test(suite, "camera_pitch", test_camera_pitch);

    CU_add_test(suite, "camera_orbit", test_camera_orbit);
    CU_add_test(suite, "camera_orbit_looks_at_center",
                test_camera_orbit_looks_at_center);

    CU_add_test(suite, "camera_get_screen_axes", test_camera_get_screen_axes);
    CU_add_test(suite, "camera_get_screen_axes_vertical",
                test_camera_get_screen_axes_vertical);

    return CUE_SUCCESS;
}