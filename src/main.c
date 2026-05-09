/**
 * \file main.c
 * \author Omar DAHMAN
 * \brief Entrypoint — handles options, launches simulation and outputs results.
 *
 * \note MODIFIED FOR LOT E (Tâche E.2):
 *   - struct arguments_s: added field \a d (depth of the environment).
 *   - handle_arguments: asks user for depth d.
 *   - main: passes d to create_environnement.
 */

#include <stdlib.h>
#include "environnement.h"
#include "interface.h"

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
 * \note MODIFIED FOR LOT E (Tâche E.2): added field \a d for depth.
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
 * \brief Collect all simulation parameters from the user via the console.
 * \return Pointer to a populated arguments struct, or NULL on allocation failure.
 * \note MODIFIED FOR LOT E (Tâche E.2): now asks for depth \a d.
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

    return options;
}

/**
 * \brief Entry point of the program.
 * \details Creates the 3D simulation environment and either:
 *   - Exports PBM images (one per iteration), or
 *   - Prints environment state to the console for each iteration.
 *
 * \note MODIFIED FOR LOT E (Tâche E.2): passes options->d to create_environnement.
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

    if (options->export_img) {
        animate(environnement, OUTPUT_DIRECTORY, options->img_w, options->img_h, options->iterations);
    } else {
        for (int i = 0; i < options->iterations; i++) {
            move_particules(environnement);
            print_environnement(environnement);
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
