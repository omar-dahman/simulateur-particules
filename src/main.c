#include <stdlib.h>

#include "environnement.h"
#include "interface.h"

/**
 * \file main.c
 * \author Maxence Raymond
 * \brief Entrypoint, handles options, launching simulation and outputting results
 */

/**
 * \brief Output directory
 * Is is defined here as there is no function to ask for a string and using scanf isn't allowed
 * \warning Filename is handled by animate functions as specified in \ref interface.h
 */
#define OUTPUT_DIRECTORY "."

/**
 * \internal
 * \brief Default message to use when none is provided
 */
#define DEFAULT_ERROR_MESSAGE "Erreur : reessayer";

/**
 * \brief Grouping of options asked to the user
 * \struct arguments_s
 * \internal
 * \see arguments
 */
struct arguments_s {
    int particle_nb; /**< Particule number of the simulation */
    int w; /**< Width of the simulation */
    int h; /**< Height of the simulation */
    float radius; /**< Radius */
    int iterations; /**< Number of iterations of the simulation */
    float iteration_t; /**< Duration of an iteration in the simulation */
    int export_img; /**< Boolean to toggle image export or rather console output */
                      /**< 1 to export, 0 to print to console */
    int img_w; /**< Image width */
               /**< Should be set to 0 if output is to be printed to console */
    int img_h; /**< Image height */
               /**< Should be set to 0 if output is to be printed to console */
};

/**
 * \typedef arguments
 * Arguments needed by the program
 * \see arguments_s
 */
typedef struct arguments_s *arguments;

/**
 * \brief Ask the user for a int with conditions
 * \param message Message to display to the user
 * \param min_value Minimum accepted value
 * \param error_msg Error message for invalid value, may be null to use default message
 * \see interface.h
 * \internal
 */

int ask_for_int(char* message, int min_value, char* error_msg) {
    int value;
    if (error_msg==NULL) {
        error_msg = DEFAULT_ERROR_MESSAGE;
    }
    print_message(message);
    value = get_int();
    while (value < min_value) {
        print_message(error_msg);
        value = get_int();
    }
    return value;
}

/**
 * \brief Ask the user for a int with even more conditions
 * \param message Message to display to the user
 * \param min_value Minimum accepted value
 * \param error_msg Error message for invalid value, may be null to use default message
 * \param max_value Maximum accepted value
 * \see ask_for_int interface.h
 * \internal
 */
int ask_for_borned_int(char* message, int min_value, char* error_msg, int max_value) {
    int value;
    do {
        value = ask_for_int(message, min_value, error_msg);
    } while (value > max_value);
    return value;
}

/**
 * \brief Ask the user for a float with conditions
 * \param message Message to display to the user
 * \param min_value Minimum accepted value
 * \param error_msg Error message for invalid value, may be null to use default message
 * \see interface.h
 * \internal
 */
float ask_for_float(char* message, float min_value, char* error_msg) {
    float value;
    if (error_msg==NULL) {
        error_msg = DEFAULT_ERROR_MESSAGE;
    }
    print_message(message);
    value = get_float();
    while (value <= min_value) {
        print_message(error_msg);
        value = get_float();
    }
    return value;
}

/**
 * \brief Populate an arguments struct by asking questions on the commande line
 * 
 * Central point to collect options by asking to the user
 * \see arguments
 * \note 
 * \parblock
 * \li Could be replaced by another function using arguments or reading from a file
 * \li Is blocking
 * \li Handles minimum and maximum values for relevant fields
 * \endparblock
 * \warning Exit on malloc failure
 */
arguments handle_arguments() {
    arguments options = malloc(sizeof(struct arguments_s));

    if(options==NULL) {
        print_message("Malloc failure");
        exit(EXIT_FAILURE);
    }

    options->particle_nb = ask_for_int("Entrer le nombre de particules :", 1, NULL);
    options->w = ask_for_int("Entrer la largeur :", 1, NULL);
    options->h = ask_for_int("Entrer la hauteur :", 1, NULL);
    options->radius = ask_for_float("Entrer le rayon :", 1, NULL);
    options->iterations = ask_for_int("Entrer le nombre d'iterations :", 0, NULL);
    options->iteration_t = ask_for_float("Entrer la duree de l'iteration :", 0, NULL);
    options->export_img = ask_for_borned_int("Entrer 0 pour la console, 1 pour exporter en image", 0, NULL, 1);
    
    // Setup these options using dummy values if using console output
    options->img_w = options->export_img==0 ? 0 : ask_for_int("Entrer la largeur du fichier image ", 1, NULL);
    options->img_h = options->export_img==0 ? 0 : ask_for_int("Entrer la hauteur du fichier image ", 1, NULL);
    
    return options;
}

/**
 * \brief Entrypoint of the program
 * 
 * Create the simulation and then either :
 * \li Output via the console informations about the state of the programs for each iteration
 * \li Output to images the representation of the simulation, one for each iteration
 */
int main(void) {
    arguments options = handle_arguments();

    env environnement = create_environnement(options->particle_nb, options->w, options->h, options->radius, options->iteration_t);

    if (options->export_img) {
        animate(environnement, OUTPUT_DIRECTORY, options->img_w, options->img_h, options->iterations);
    } else {
        for (int i = 0; i < options->iterations; i++) {
            move_particules(environnement);
            print_environnement(environnement);
        }   
    }
    free(options);
    print_message("Simulation terminee");

    return EXIT_SUCCESS;
}


/**
 * \mainpage Index Page
 * Welcome to the simparticles project ! 
 * 
 * Usage : ./simparticles  
 * Just run it, it will ask all the necessary questions
 * 
 * Go check out \link headers
 */