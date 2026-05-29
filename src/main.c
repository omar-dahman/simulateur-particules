/**
 * \file main.c
 * \author Omar DAHMAN
 * \brief Entrypoint — handles options, launches simulation and outputs results.
 *
 * \note MODIFIED FOR LOT E (Tâche E.2):
 *   - struct options_s: added field \a d (depth of the environment).
 *   - handle_arguments: asks user for depth d.
 *   - main: passes d to create_environnement.
 *
 * \note MODIFIED FOR LOT E (Tâche E.3):
 *   - struct options_s: added camera position, direction and movement fields.
 *   - ask_for_any_float: new helper for floats with no lower bound.
 *   - ask_camera_parameters: new function to collect camera configuration.
 *   - create_camera_from_options: new function to build a Camera from options.
 *   - handle_arguments: now calls ask_camera_parameters.
 *   - main: creates Camera and passes it to animate / camera_print.
 */

#include <stdlib.h>
#include "environnement.h"
#include "interface.h"
#include "camera.h"
#include "vector.h"
#include "options.h"

/**
 * \brief Output directory for PBM images.
 * \warning Filenames are handled by animate() as specified in interface.h.
 */
#define OUTPUT_DIRECTORY "output"

/** \internal Default error message used when none is provided. */
#define DEFAULT_ERROR_MESSAGE "Erreur : reessayer"

/**
 * \internal
 * \brief Ask the user for an integer with a minimum value constraint.
 * \param message    Prompt to display.
 * \param min_value  Minimum accepted value.
 * \param error_msg  Error message (NULL = use default).
 * \return Valid integer >= min_value.
 */
static int ask_for_int(char *message, int min_value, char *error_msg)
{
    int value;
    if (error_msg == NULL)
        error_msg = DEFAULT_ERROR_MESSAGE;
    print_message(message);
    value = get_int();
    while (value < min_value)
    {
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
static int ask_for_borned_int(char *message, int min_value, char *error_msg, int max_value)
{
    int value;
    do
    {
        value = ask_for_int(message, min_value, error_msg);
    } while (value > max_value);
    return value;
}

/**
 * \internal
 * \brief Ask the user for a float strictly greater than a minimum value.
 * \details Use this for quantities that must be strictly positive (speeds,
 *          radii, durations). For coordinates or direction components that
 *          may be zero or negative, use ask_for_any_float() instead.
 * \param message    Prompt to display.
 * \param min_value  Exclusive lower bound.
 * \param error_msg  Error message for invalid value, may be null to use default message.
 * \return Valid float > min_value.
 */
static float ask_for_float(char *message, float min_value, char *error_msg)
{
    float value;
    if (error_msg == NULL)
        error_msg = DEFAULT_ERROR_MESSAGE;
    print_message(message);
    value = get_float();
    while (value <= min_value)
    {
        print_message(error_msg);
        value = get_float();
    }
    return value;
}

/**
 * \internal
 * \brief Ask the user for any float value (no constraint).
 * \details Use this for coordinates, direction components, or any value that
 *          may be zero or negative (e.g. camera position, gaze direction,
 *          orbit centre).
 * \param message Prompt to display.
 * \return Float value entered by the user.
 *
 * \note ADDED FOR LOT E (Tâche E.3): needed because ask_for_float() rejects
 *       values ≤ its min_value, which would make negative coordinates or
 *       direction components like (−1, 0, 0) impossible to enter.
 */
static float ask_for_any_float(char *message)
{
    print_message(message);
    return get_float();
}

/**
 * \internal
 * \brief Ask the user for all camera configuration parameters and store
 *        them in \a options.
 * \param options Pointer to the arguments struct to populate.
 *
 * \note ADDED FOR LOT E (Tâche E.3): entire function is new.
 */
static void ask_camera_parameters(options options)
{
    print_message("\n--- Configuration de la camera ---\n");

    /* Camera position: any real coordinate is valid */
    options->cam_x = ask_for_any_float("Position X de la camera : ");
    options->cam_y = ask_for_any_float("Position Y de la camera : ");
    options->cam_z = ask_for_any_float("Position Z de la camera : ");

    /* Gaze direction: any non-zero vector (will be normalised by camera_create) */
    options->cam_dir_x = ask_for_any_float("Direction X : ");
    options->cam_dir_y = ask_for_any_float("Direction Y : ");
    options->cam_dir_z = ask_for_any_float("Direction Z : ");

    options->camera_move_type = ask_for_borned_int(
        "Type mouvement (0:static,1:translate,2:orbit,3:fly,4:pendulum) : ", 0, NULL, 4);

    switch (options->camera_move_type)
    {
    case 1: /* Translation */
        options->trans_dir_x = ask_for_any_float("Dir X translation : ");
        options->trans_dir_y = ask_for_any_float("Dir Y : ");
        options->trans_dir_z = ask_for_any_float("Dir Z : ");
        options->trans_speed  = ask_for_float("Vitesse : ", 0.0f, NULL);
        break;

    case 2: /* Orbit */
        options->orbit_center_x      = ask_for_any_float("Centre X : ");
        options->orbit_center_y      = ask_for_any_float("Centre Y : ");
        options->orbit_center_z      = ask_for_any_float("Centre Z : ");
        options->orbit_radius        = ask_for_float("Rayon : ", 0.0f, NULL);
        options->orbit_angular_speed = ask_for_float("Vitesse angulaire : ", 0.0f, NULL);
        options->orbit_elevation     = ask_for_any_float("Elevation (rad) : ");
        break;

    case 3: /* Fly */
        options->fly_target_x  = ask_for_any_float("Cible X : ");
        options->fly_target_y  = ask_for_any_float("Cible Y : ");
        options->fly_target_z  = ask_for_any_float("Cible Z : ");
        options->fly_speed     = ask_for_float("Vitesse : ", 0.0f, NULL);
        options->fly_turn_rate = ask_for_float("Taux rotation (0-1) : ", 0.0f, NULL);
        break;

    case 4: /* Pendulum */
        options->pend_pivot_x    = ask_for_any_float("Pivot X : ");
        options->pend_pivot_y    = ask_for_any_float("Pivot Y : ");
        options->pend_pivot_z    = ask_for_any_float("Pivot Z : ");
        options->pend_radius     = ask_for_float("Rayon : ", 0.0f, NULL);
        options->pend_amplitude  = ask_for_float("Amplitude : ", 0.0f, NULL);
        options->pend_frequency  = ask_for_float("Frequence : ", 0.0f, NULL);
        break;

    default:
        break;
    }
}

/**
 * \internal
 * \brief Builds a Camera from the parameters stored in \a options.
 * \param options Populated options struct.
 * \return Configured Camera value.
 *
 * \note ADDED FOR LOT E (Tâche E.3): entire function is new.
 */
static Camera create_camera_from_options(options options)
{
    vec3   pos = vec3_make(options->cam_x, options->cam_y, options->cam_z);
    vec3   dir = vec3_make(options->cam_dir_x, options->cam_dir_y, options->cam_dir_z);
    Camera cam = camera_create(pos, dir, options->img_w, options->img_h);

    switch (options->camera_move_type)
    {
    case 1:
        camera_set_move_translate(
            &cam,
            vec3_make(options->trans_dir_x, options->trans_dir_y, options->trans_dir_z),
            options->trans_speed);
        break;

    case 2:
        camera_set_move_orbit(
            &cam,
            vec3_make(options->orbit_center_x, options->orbit_center_y, options->orbit_center_z),
            options->orbit_radius,
            options->orbit_angular_speed,
            options->orbit_elevation);
        break;

    case 3:
        camera_set_move_fly(
            &cam,
            vec3_make(options->fly_target_x, options->fly_target_y, options->fly_target_z),
            options->fly_speed,
            options->fly_turn_rate);
        break;

    case 4:
        camera_set_move_pendulum(
            &cam,
            vec3_make(options->pend_pivot_x, options->pend_pivot_y, options->pend_pivot_z),
            options->pend_radius,
            options->pend_amplitude,
            options->pend_frequency);
        break;

    default:
        camera_set_move_none(&cam);
        break;
    }

    return cam;
}

/**
 * \brief Collect all simulation parameters from the user via the console.
 * \return Pointer to a populated options struct, or NULL on allocation failure.
 * \note MODIFIED FOR LOT E (Tâche E.2): now asks for depth \a d.
 * \note MODIFIED FOR LOT E (Tâche E.3): now calls ask_camera_parameters().
 */
static options handle_arguments(void)
{
    options options = malloc(sizeof(struct options_s));
    if (options == NULL)
        return NULL;

    options->particle_nb  = ask_for_int("Entrer le nombre de particules : ", 1, NULL);
    options->w            = ask_for_int("Entrer la largeur (w) : ", 1, NULL);
    options->h            = ask_for_int("Entrer la hauteur (h) : ", 1, NULL);
    options->d            = ask_for_int("Entrer la profondeur (d) : ", 1, NULL); /* ADDED FOR LOT E - Tâche E.2 */
    options->radius       = ask_for_float("Entrer le rayon d'interaction : ", 0.0f, NULL);
    options->iterations   = ask_for_int("Entrer le nombre d'iterations : ", 0, NULL);
    options->iteration_t  = ask_for_float("Entrer la duree de l'iteration (dt) : ", 0.0f, NULL);
    options->export_img   = ask_for_borned_int(
        "Entrer 0 pour la console, 1 pour exporter en images PBM : ", 0, NULL, 1);

    if (options->export_img == 0)
    {
        options->img_w = 0;
        options->img_h = 0;
    }
    else
    {
        options->img_w = ask_for_int("Entrer la largeur de l'image (pixels) : ", 1, NULL);
        options->img_h = ask_for_int("Entrer la hauteur de l'image (pixels) : ", 1, NULL);
    }

    ask_camera_parameters(options); /* ADDED FOR LOT E - Tâche E.3 */
    return options;
}

/**
 * \brief Entry point of the program.
 * \details Creates the 3D simulation environment and either:
 *   - Exports PBM images (one per iteration), or
 *   - Prints environment state to the console for each iteration.
 *
 * \note MODIFIED FOR LOT E (Tâche E.2): passes options->d to create_environnement.
 * \note MODIFIED FOR LOT E (Tâche E.3): creates Camera and passes it to
 *       animate() / camera_print().
 */
int main(void)
{
    options options = handle_arguments();
    if (options == NULL)
    {
        print_message("Erreur d'allocation memoire\n");
        return EXIT_FAILURE;
    }

    env environnement = create_environnement(
        options->particle_nb,
        (float)options->w,
        (float)options->h,
        (float)options->d,   /* ADDED FOR LOT E - Tâche E.2 */
        options->radius,
        options->iteration_t);

    if (environnement == NULL)
    {
        print_message("Echec de la creation de l'environnement\n");
        free(options);
        return EXIT_FAILURE;
    }

    Camera cam = create_camera_from_options(options); /* ADDED FOR LOT E - Tâche E.3 */

    if (options->export_img)
    {
        animate(environnement, OUTPUT_DIRECTORY,
                options->img_w, options->img_h,
                options->iterations, &cam);
    }
    else
    {
        for (int i = 0; i < options->iterations; i++)
        {
            move_particules(environnement);
            print_environnement(environnement);
            camera_update(&cam);
            camera_print(&cam); /* ADDED FOR LOT E - Tâche E.3 */
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
