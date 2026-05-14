/**
 * \file main.c
 * \author Omar DAHMAN
 * \brief Entrypoint — handles options, launches simulation and outputs results.
 *
 * \note MODIFIED FOR LOT E (Tâche E.2):
 *   - struct arguments_s: added field \a d (depth of the environment).
 *   - handle_arguments: asks user for depth d.
 *   - main: passes d to create_environnement.
 *
 * \note MODIFIED FOR LOT E (Tâche E.3):
 *   - struct arguments_s: added fields for camera position, direction and movement choice.
 *   - handle_camera_arguments: new function that asks the user to configure the camera.
 *   - setup_camera: new function that applies the chosen movement to the camera.
 *   - main: camera is now fully configured from user input before the simulation starts.
 */

#include <stdlib.h>
#include "environnement.h"
#include "interface.h"
#include "camera.h"
#include "vector.h"

/**
 * \brief Output directory for PBM images.
 * \warning Filenames are handled by animate() as specified in interface.h.
 */
#define OUTPUT_DIRECTORY "output"

/** \internal Default error message used when none is provided. */
#define DEFAULT_ERROR_MESSAGE "Erreur : reessayer"

/**
 * \brief Grouping of simulation options asked to the user.
 * \struct arguments_s
 * \internal

 */
struct arguments_s {
    int   particle_nb;  /**< Number of particles */
    int   w;            /**< Width  of the environment (x-axis) */
    int   h;            /**< Height of the environment (y-axis) */
    int   d;            /**< Depth  of the environment (z-axis) [ADDED FOR LOT E - Tâche E.2] */
    float radius;       /**< Interaction radius */
    int   iterations;   /**< Number of simulation iterations */
    float iteration_t;  /**< Duration of one iteration */
    int   export_img;   /**< 1 = export PBM images, 0 = print to console */
    int   img_w;        /**< Image width  in pixels (0 if console output) */
    int   img_h;        /**< Image height in pixels (0 if console output) */

    /* Camera configuration [ADDED FOR LOT E - Tâche E.3] */
    int   cam_move;     /**< Camera movement type chosen by the user (0..4) */
    float cam_px;       /**< Camera initial position x */
    float cam_py;       /**< Camera initial position y */
    float cam_pz;       /**< Camera initial position z */
    float cam_dx;       /**< Camera initial direction x */
    float cam_dy;       /**< Camera initial direction y */
    float cam_dz;       /**< Camera initial direction z */
    /* Parameters for TRANSLATE */
    float cam_speed;    /**< Speed (used by TRANSLATE and FLY) */
    /* Parameters for ORBIT */
    float cam_cx;       /**< Orbit/pendulum center x */
    float cam_cy;       /**< Orbit/pendulum center y */
    float cam_cz;       /**< Orbit/pendulum center z */
    float cam_radius;   /**< Orbit/pendulum radius */
    float cam_ang_spd;  /**< Orbit angular speed */
    float cam_elev;     /**< Orbit elevation */
    /* Parameters for FLY */
    float cam_tx;       /**< Fly target x */
    float cam_ty;       /**< Fly target y */
    float cam_tz;       /**< Fly target z */
    float cam_turn;     /**< Fly turn rate */
    /* Parameters for PENDULUM */
    float cam_ampl;     /**< Pendulum amplitude */
    float cam_freq;     /**< Pendulum frequency */
};

/**
 * \typedef arguments
 * \see arguments_s
 */
typedef struct arguments_s *arguments;

/**
 * \internal
 * \brief Ask the user for an integer with a minimum value constraint.
 * \param message    Prompt to display.
 * \param min_value  Minimum accepted value.
 * \param error_msg  Error message (NULL = use default).
 * \return Valid integer >= min_value.
 */
static int ask_for_int(char *message, int min_value, char *error_msg) {
    int value;
    if (error_msg == NULL) error_msg = DEFAULT_ERROR_MESSAGE;
    print_message(message);
    value = get_int();
    while (value < min_value) {
        print_message(error_msg);
        value = get_int();
    }
    return value;
}

/**
 * \internal
 * \brief Ask the user for an integer bounded between min and max.
 * \param message    Prompt to display.
 * \param min_value  Minimum accepted value.
 * \param error_msg  Error message for invalid value, may be null to use default message.
 * \param max_value  Maximum accepted value.
 * \return Valid integer in [min_value, max_value].
 */
static int ask_for_borned_int(char *message, int min_value, char *error_msg, int max_value) {
    int value;
    do {
        value = ask_for_int(message, min_value, error_msg);
    } while (value > max_value);
    return value;
}

/**
 * \internal
 * \brief Ask the user for a float strictly greater than a minimum value.
 * \param message    Prompt to display.
 * \param min_value  Exclusive lower bound.
 * \param error_msg  Error message for invalid value, may be null to use default message.
 * \return Valid float > min_value.
 */
static float ask_for_float(char *message, float min_value, char *error_msg) {
    float value;
    if (error_msg == NULL) error_msg = DEFAULT_ERROR_MESSAGE;
    print_message(message);
    value = get_float();
    while (value <= min_value) {
        print_message(error_msg);
        value = get_float();
    }
    return value;
}

/**
 * \brief Ask the user for any float (no lower bound constraint).
 * \param message Prompt to display.
 * \return The entered float value.
 */
static float ask_for_any_float(char *message) {
    print_message(message);
    return get_float();
}

/**
 * \brief Collect camera configuration from the user.
 * \details Asks for the initial position, direction, then the desired
 *          movement type and its associated parameters.
 * \param options Pointer to the arguments struct to fill.
 * \note ADDED FOR LOT E (Tâche E.3)
 */
static void handle_camera_arguments(arguments options) {
    print_message("\n--- Configuration de la camera ---\n");

    /* Initial position */
    print_message("Position initiale de la camera :\n");
    options->cam_px = ask_for_any_float("  x : ");
    options->cam_py = ask_for_any_float("  y : ");
    options->cam_pz = ask_for_any_float("  z : ");

    /* Initial gaze direction */
    print_message("Direction de visee (vecteur, sera normalise) :\n");
    options->cam_dx = ask_for_any_float("  dx : ");
    options->cam_dy = ask_for_any_float("  dy : ");
    options->cam_dz = ask_for_any_float("  dz : ");

    /* Movement type */
    print_message("\nType de mouvement de la camera :\n");
    print_message("  0 - Aucun (camera fixe)\n");
    print_message("  1 - Translation en ligne droite\n");
    print_message("  2 - Orbite autour d'un point\n");
    print_message("  3 - Survol vers une cible (fly)\n");
    print_message("  4 - Balancement (pendulum)\n");
    options->cam_move = ask_for_borned_int("Votre choix : ", 0, NULL, 4);

    /* Parameters depending on the chosen movement */
    switch (options->cam_move) {

        case CAMERA_MOVE_NONE:
            /* No extra parameters needed */
            break;

        case CAMERA_MOVE_TRANSLATE:
            print_message("Direction de deplacement (vecteur, sera normalise) :\n");
            options->cam_dx  = ask_for_any_float("  dx : ");
            options->cam_dy  = ask_for_any_float("  dy : ");
            options->cam_dz  = ask_for_any_float("  dz : ");
            options->cam_speed = ask_for_float("Vitesse (> 0) : ", 0.0f, NULL);
            break;

        case CAMERA_MOVE_ORBIT:
            print_message("Centre de l'orbite :\n");
            options->cam_cx = ask_for_any_float("  cx : ");
            options->cam_cy = ask_for_any_float("  cy : ");
            options->cam_cz = ask_for_any_float("  cz : ");
            options->cam_radius  = ask_for_float("Rayon de l'orbite (> 0) : ", 0.0f, NULL);
            options->cam_ang_spd = ask_for_float("Vitesse angulaire en rad/iteration (> 0) : ", 0.0f, NULL);
            options->cam_elev    = ask_for_any_float("Elevation en radians : ");
            break;

        case CAMERA_MOVE_FLY:
            print_message("Point cible :\n");
            options->cam_tx   = ask_for_any_float("  tx : ");
            options->cam_ty   = ask_for_any_float("  ty : ");
            options->cam_tz   = ask_for_any_float("  tz : ");
            options->cam_speed = ask_for_float("Vitesse (> 0) : ", 0.0f, NULL);
            options->cam_turn  = ask_for_float("Taux de rotation vers la cible (> 0) : ", 0.0f, NULL);
            break;

        case CAMERA_MOVE_PENDULUM:
            print_message("Centre du balancement (pivot) :\n");
            options->cam_cx   = ask_for_any_float("  cx : ");
            options->cam_cy   = ask_for_any_float("  cy : ");
            options->cam_cz   = ask_for_any_float("  cz : ");
            options->cam_radius = ask_for_float("Distance pivot-camera (> 0) : ", 0.0f, NULL);
            options->cam_ampl   = ask_for_float("Amplitude en radians (> 0) : ", 0.0f, NULL);
            options->cam_freq   = ask_for_float("Frequence en rad/iteration (> 0) : ", 0.0f, NULL);
            break;
    }
}

/**
 * \internal
 * \brief Creates and configures a Camera from the collected arguments.
 * \param options Simulation options containing camera parameters.
 * \param img_w   Screen width  in pixels.
 * \param img_h   Screen height in pixels.
 * \return Fully configured Camera.
 * \note ADDED FOR LOT E (Tâche E.3) 
 */
static Camera setup_camera(arguments options, int img_w, int img_h) {
    vec3   position  = vec3_make(options->cam_px, options->cam_py, options->cam_pz);
    vec3   direction = vec3_make(options->cam_dx, options->cam_dy, options->cam_dz);
    Camera cam       = camera_create(position, direction, img_w, img_h);

    switch (options->cam_move) {

        case CAMERA_MOVE_NONE:
            camera_set_move_none(&cam);
            break;

        case CAMERA_MOVE_TRANSLATE: {
            vec3 dir = vec3_make(options->cam_dx, options->cam_dy, options->cam_dz);
            camera_set_move_translate(&cam, dir, options->cam_speed);
            break;
        }

        case CAMERA_MOVE_ORBIT: {
            vec3 center = vec3_make(options->cam_cx, options->cam_cy, options->cam_cz);
            camera_set_move_orbit(&cam, center, options->cam_radius,
                                  options->cam_ang_spd, options->cam_elev);
            break;
        }

        case CAMERA_MOVE_FLY: {
            vec3 target = vec3_make(options->cam_tx, options->cam_ty, options->cam_tz);
            camera_set_move_fly(&cam, target, options->cam_speed, options->cam_turn);
            break;
        }

        case CAMERA_MOVE_PENDULUM: {
            vec3 pivot = vec3_make(options->cam_cx, options->cam_cy, options->cam_cz);
            camera_set_move_pendulum(&cam, pivot, options->cam_radius,
                                     options->cam_ampl, options->cam_freq);
            break;
        }
    }

    return cam;
}

/**
 * \brief Collect all simulation parameters from the user via the console.
 * \return Pointer to a populated arguments struct, or NULL on allocation failure.
 * \note MODIFIED FOR LOT E (Tâche E.2): now asks for depth \a d.
 * \note MODIFIED FOR LOT E (Tâche E.3): now asks for camera configuration.
 */
static arguments handle_arguments(void) {
    arguments options = malloc(sizeof(struct arguments_s));
    if (options == NULL) return NULL;

    options->particle_nb = ask_for_int("Entrer le nombre de particules : ", 1, NULL);
    options->w           = ask_for_int("Entrer la largeur (w) : ", 1, NULL);
    options->h           = ask_for_int("Entrer la hauteur (h) : ", 1, NULL);
    /* ADDED FOR LOT E (Tâche E.2): depth parameter */
    options->d           = ask_for_int("Entrer la profondeur (d) : ", 1, NULL);
    options->radius      = ask_for_float("Entrer le rayon d'interaction : ", 0.0f, NULL);
    options->iterations  = ask_for_int("Entrer le nombre d'iterations : ", 0, NULL);
    options->iteration_t = ask_for_float("Entrer la duree de l'iteration (dt) : ", 0.0f, NULL);
    options->export_img  = ask_for_borned_int(
        "Entrer 0 pour la console, 1 pour exporter en images PBM : ", 0, NULL, 1);

    if (options->export_img == 0) {
        options->img_w = 0;
        options->img_h = 0;
    } else {
        options->img_w = ask_for_int("Entrer la largeur de l'image (pixels) : ", 1, NULL);
        options->img_h = ask_for_int("Entrer la hauteur de l'image (pixels) : ", 1, NULL);
    }

    /* ADDED FOR LOT E (Tâche E.3): camera configuration */
    handle_camera_arguments(options);

    return options;
}

/**
 * \brief Entry point of the program.
 * \details Creates the 3D simulation environment and either:
 *   - Exports PBM images (one per iteration), or
 *   - Prints environment state to the console for each iteration.
 *
 * \note MODIFIED FOR LOT E (Tâche E.2): passes options->d to create_environnement.
 * \note MODIFIED FOR LOT E (Tâche E.3): camera is configured from user input.
 */
int main(void) {
    arguments options = handle_arguments();
    if (options == NULL) {
        print_message("Erreur d'allocation memoire\n");
        return EXIT_FAILURE;
    }

    /* MODIFIED FOR LOT E (Tâche E.2): create_environnement now takes depth d */
    env environnement = create_environnement(
        options->particle_nb,
        (float)options->w,
        (float)options->h,
        (float)options->d,   /* ADDED FOR LOT E (Tâche E.2) */
        options->radius,
        options->iteration_t
    );

    if (environnement == NULL) {
        print_message("Echec de la creation de l'environnement\n");
        free(options);
        return EXIT_FAILURE;
    }

    /* ADDED FOR LOT E (Tâche E.3): create and configure camera from user input */
    Camera cam = setup_camera(options, options->img_w, options->img_h);

    if (options->export_img) {
        animate(environnement, OUTPUT_DIRECTORY, options->img_w, options->img_h, options->iterations, &cam);
    } else {
        for (int i = 0; i < options->iterations; i++) {
            move_particules(environnement);
            print_environnement(environnement);
            camera_update(&cam);   /* ADDED FOR LOT E (Tâche E.3) */
            camera_print(&cam);    /* ADDED FOR LOT E (Tâche E.3) */
        }
    }

    free(options);
    free_environnement(environnement);
    print_message("Simulation terminee\n");

    return EXIT_SUCCESS;
}

/**
 * \mainpage Index Page
 * Welcome to the simparticles project !
 *
 * Usage: ./simparticles
 * The program will prompt for all necessary parameters.
 *
 * \see headers/
 */
