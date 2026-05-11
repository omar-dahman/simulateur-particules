/**
 * \file    test_vector.c
 * \author  Othmane CHAOUI
 * \date    2026-05-11
 *
 * \brief   Tests of 3D and vector library
 * \details This file contains the tests on changes made
 *          by \b Task \b E.2
 */

#include <math.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "vector.h"

#define EPSILON 1e-6f
#define M_PI 3.14159265358979323846

void test_vec3_make()
{
    vec3 v = vec3_make(1.0f, 2.0f, 3.0f);
    CU_ASSERT_DOUBLE_EQUAL(v.x, 1.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(v.y, 2.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(v.z, 3.0f, EPSILON);
}

void test_vec3_copy()
{
    vec3 v = {4.0f, 5.0f, 6.0f};
    vec3 c = vec3_copy(v);
    CU_ASSERT_DOUBLE_EQUAL(c.x, 4.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(c.y, 5.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(c.z, 6.0f, EPSILON);
}

void test_vec3_add()
{
    vec3 a = {1.0f, 2.0f, 3.0f};
    vec3 b = {4.0f, 5.0f, 6.0f};
    vec3 c = vec3_add(a, b);
    CU_ASSERT_DOUBLE_EQUAL(c.x, 5.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(c.y, 7.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(c.z, 9.0f, EPSILON);
}

void test_vec3_sub()
{
    vec3 a = {4.0f, 5.0f, 6.0f};
    vec3 b = {1.0f, 2.0f, 3.0f};
    vec3 c = vec3_sub(a, b);
    CU_ASSERT_DOUBLE_EQUAL(c.x, 3.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(c.y, 3.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(c.z, 3.0f, EPSILON);
}

void test_vec3_scale()
{
    vec3 v = {2.0f, 3.0f, 4.0f};
    vec3 r = vec3_scale(v, 2.5f);
    CU_ASSERT_DOUBLE_EQUAL(r.x, 5.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(r.y, 7.5f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(r.z, 10.0f, EPSILON);
}

void test_vec3_negate()
{
    vec3 v = {1.0f, -2.0f, 3.0f};
    vec3 n = vec3_negate(v);
    CU_ASSERT_DOUBLE_EQUAL(n.x, -1.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(n.y, 2.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(n.z, -3.0f, EPSILON);
}

void test_vec3_norm()
{
    vec3 v = {3.0f, 4.0f, 0.0f};
    float norm = vec3_norm(v);
    CU_ASSERT_DOUBLE_EQUAL(norm, 5.0f, EPSILON);
}

void test_vec3_norm2()
{
    vec3 v = {3.0f, 4.0f, 0.0f};
    float norm2 = vec3_norm2(v);
    CU_ASSERT_DOUBLE_EQUAL(norm2, 25.0f, EPSILON);
}

void test_vec3_normalize()
{
    vec3 v = {3.0f, 4.0f, 0.0f};
    vec3 n = vec3_normalize(v);
    float norm = vec3_norm(n);
    CU_ASSERT_DOUBLE_EQUAL(norm, 1.0f, EPSILON);
}

void test_vec3_normalize_zero()
{
    vec3 zero = {0.0f, 0.0f, 0.0f};
    vec3 n = vec3_normalize(zero);
    CU_ASSERT_DOUBLE_EQUAL(n.x, 0.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(n.y, 0.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(n.z, 0.0f, EPSILON);
}

void test_vec3_dot()
{
    vec3 a = {1.0f, 0.0f, 0.0f};
    vec3 b = {0.0f, 1.0f, 0.0f};
    float dot = vec3_dot(a, b);
    CU_ASSERT_DOUBLE_EQUAL(dot, 0.0f, EPSILON);
}

void test_vec3_cross()
{
    vec3 a = {1.0f, 0.0f, 0.0f};
    vec3 b = {0.0f, 1.0f, 0.0f};
    vec3 c = vec3_cross(a, b);
    CU_ASSERT_DOUBLE_EQUAL(c.x, 0.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(c.y, 0.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(c.z, 1.0f, EPSILON);
}

void test_vec3_angle()
{
    vec3 a = {1.0f, 0.0f, 0.0f};
    vec3 b = {0.0f, 1.0f, 0.0f};
    float angle = vec3_angle(a, b);
    CU_ASSERT_DOUBLE_EQUAL(angle, M_PI / 2.0, EPSILON);
}

void test_vec3_distance()
{
    vec3 a = {1.0f, 0.0f, 0.0f};
    vec3 b = {4.0f, 4.0f, 0.0f};
    float dist = vec3_distance(a, b);
    CU_ASSERT_DOUBLE_EQUAL(dist, 5.0f, EPSILON);
}

void test_vec3_project()
{
    vec3 v = {3.0f, 4.0f, 0.0f};
    vec3 onto = {1.0f, 0.0f, 0.0f};
    vec3 proj = vec3_project(v, onto);
    CU_ASSERT_DOUBLE_EQUAL(proj.x, 3.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(proj.y, 0.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(proj.z, 0.0f, EPSILON);
}

void test_vec3_reflect()
{
    vec3 v = {1.0f, -1.0f, 0.0f};
    vec3 n = {0.0f, 1.0f, 0.0f};
    vec3 r = vec3_reflect(v, n);
    CU_ASSERT_DOUBLE_EQUAL(r.x, 1.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(r.y, 1.0f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(r.z, 0.0f, EPSILON);
}

void test_vec3_equal()
{
    vec3 a = {1.0f, 2.0f, 3.0f};
    vec3 b = {1.0001f, 2.0001f, 3.0001f};
    int eq = vec3_equal(a, b, 1e-3f);
    CU_ASSERT_TRUE(eq);
}

CU_ErrorCode test_vector_register_suite()
{
    CU_pSuite suite = CU_add_suite("Vector 3D", NULL, NULL);
    if (!suite)
        return CU_get_error();

    CU_add_test(suite, "vec3_make", test_vec3_make);
    CU_add_test(suite, "vec3_copy", test_vec3_copy);

    CU_add_test(suite, "vec3_add", test_vec3_add);
    CU_add_test(suite, "vec3_sub", test_vec3_sub);
    CU_add_test(suite, "vec3_scale", test_vec3_scale);
    CU_add_test(suite, "vec3_negate", test_vec3_negate);

    CU_add_test(suite, "vec3_norm", test_vec3_norm);
    CU_add_test(suite, "vec3_norm2", test_vec3_norm2);
    CU_add_test(suite, "vec3_normalize", test_vec3_normalize);
    CU_add_test(suite, "vec3_normalize_zero", test_vec3_normalize_zero);

    CU_add_test(suite, "vec3_dot", test_vec3_dot);
    CU_add_test(suite, "vec3_cross", test_vec3_cross);

    CU_add_test(suite, "vec3_angle", test_vec3_angle);
    CU_add_test(suite, "vec3_distance", test_vec3_distance);
    CU_add_test(suite, "vec3_project", test_vec3_project);
    CU_add_test(suite, "vec3_reflect", test_vec3_reflect);

    CU_add_test(suite, "vec3_equal", test_vec3_equal);

    return CUE_SUCCESS;
}