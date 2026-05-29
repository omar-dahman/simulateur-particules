#include <stdlib.h>
#include "options.h"

options init_empty_options() {
    options opt = malloc(sizeof(struct options_s));

    if (opt == NULL) {
        return NULL;
    }

    opt->particle_nb = 0;
    opt->w = 0;
    opt->h = 0;
    opt->d = 0;
    opt->radius = 0.0f;
    opt->iterations = 0;
    opt->iteration_t = 0.0f;
    opt->export_img = 0;
    opt->img_w = 0;
    opt->img_h = 0;

    /* Camera parameters */
    opt->cam_x = 0.0f;
    opt->cam_y = 0.0f;
    opt->cam_z = 0.0f;
    opt->cam_dir_x = 0.0f;
    opt->cam_dir_y = 0.0f;
    opt->cam_dir_z = 0.0f;
    opt->camera_move_type = 0;

    /* Translation movement parameters */
    opt->trans_dir_x = 0.0f;
    opt->trans_dir_y = 0.0f;
    opt->trans_dir_z = 0.0f;
    opt->trans_speed = 0.0f;

    /* Orbit movement parameters */
    opt->orbit_center_x = 0.0f;
    opt->orbit_center_y = 0.0f;
    opt->orbit_center_z = 0.0f;
    opt->orbit_radius = 0.0f;
    opt->orbit_angular_speed = 0.0f;
    opt->orbit_elevation = 0.0f;

    /* Fly movement parameters */
    opt->fly_target_x = 0.0f;
    opt->fly_target_y = 0.0f;
    opt->fly_target_z = 0.0f;
    opt->fly_speed = 0.0f;
    opt->fly_turn_rate = 0.0f;

    /* Pendulum movement parameters */
    opt->pend_pivot_x = 0.0f;
    opt->pend_pivot_y = 0.0f;
    opt->pend_pivot_z = 0.0f;
    opt->pend_radius = 0.0f;
    opt->pend_amplitude = 0.0f;
    opt->pend_frequency = 0.0f;

    return opt;
}