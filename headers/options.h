#ifndef OPTIONS_H
#define OPTIONS_H

/**
 * \brief Grouping of simulation options asked to the user.
 * \struct options_s
 * \internal
 * \note MODIFIED FOR LOT E (Tâche E.2): added field \a d for depth.
 * \note MODIFIED FOR LOT E (Tâche E.3): added camera position, direction and
 *       movement parameters (cam_x … pend_frequency).
 */
struct options_s
{
    int particle_nb;   /**< Number of particles */
    int w;             /**< Width  of the environment (x-axis) */
    int h;             /**< Height of the environment (y-axis) */
    int d;             /**< Depth  of the environment (z-axis) [ADDED FOR LOT E - Tâche E.2] */
    float radius;      /**< Interaction radius */
    int iterations;    /**< Number of simulation iterations */
    float iteration_t; /**< Duration of one iteration */
    int export_img;    /**< 1 = export PBM images, 0 = print to console */
    int img_w;         /**< Image width  in pixels (0 if console output) */
    int img_h;         /**< Image height in pixels (0 if console output) */

    /* ADDED FOR LOT E (Tâche E.3): camera parameters */
    float cam_x, cam_y, cam_z;               /**< Camera position in 3D space     */
    float cam_dir_x, cam_dir_y, cam_dir_z;   /**< Camera gaze direction (any unit) */
    int   camera_move_type;                  /**< Movement type (0-4)              */

    /* Translation movement parameters [ADDED FOR LOT E - Tâche E.3] */
    float trans_dir_x, trans_dir_y, trans_dir_z;
    float trans_speed;

    /* Orbit movement parameters [ADDED FOR LOT E - Tâche E.3] */
    float orbit_center_x, orbit_center_y, orbit_center_z;
    float orbit_radius, orbit_angular_speed, orbit_elevation;

    /* Fly movement parameters [ADDED FOR LOT E - Tâche E.3] */
    float fly_target_x, fly_target_y, fly_target_z;
    float fly_speed, fly_turn_rate;

    /* Pendulum movement parameters [ADDED FOR LOT E - Tâche E.3] */
    float pend_pivot_x, pend_pivot_y, pend_pivot_z;
    float pend_radius, pend_amplitude, pend_frequency;
};

/**
 * \typedef options
 * \see options_s
 */
typedef struct options_s *options;

options init_empty_options();
#endif