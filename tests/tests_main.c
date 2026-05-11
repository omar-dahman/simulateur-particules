/**
 * \file    tests_main.c
 * \author  Othmane CHAOUI
 * \date    2026-04-23
 *
 * \brief   Tests's main
 * \details This file contains the main in which the tests
 *          will be executed
 */

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

extern CU_ErrorCode test_vector_register_suite();
// extern CU_ErrorCode test_forces_register_suite();
// extern CU_ErrorCode test_movement_camera_register_suite(void);
// extern CU_ErrorCode test_vision_camera_register_suite(void);

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        fprintf(stderr, "Error : failled to initialize CUnit\n");
        return EXIT_FAILURE;
    }

    test_vector_register_suite();
    // test_forces_register_suite();
    // test_movement_camera_register_suite();
    // test_vision_camera_register_suite();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_number_of_failures() == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}