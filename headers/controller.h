/**
 * \file controller.h
 * \author Nell TELECHEA
 * 
 * \brief Controller part of the interface
 * \details This file contains declarations for functions that will link
 * 			the graphical user interface and the particle simulator.
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <gtk/gtk.h> /*The library used for the interface.*/
#include "app.h" /*The interface*/

/**
 * \brief Callback for the "Add obstacle" button
 * \param btn The button that was clicked
 * \param user_data Pointer to AppData structure
 */
void on_add_obstacle(GtkButton *btn, gpointer user_data);

/**
 * \brief Advances the simulation by one iteration.
 * \param btn The button that triggered the callback.
 * \param user_data The pointer to the current application data.
 */
void on_next(GtkButton *btn, gpointer user_data);

/**
 * \brief Creates the simulation environment using the values currently entered in the interface.
 * \param btn The button that triggered the callback.
 * \param user_data The pointer tot the current application data.
 */
void on_create(GtkButton *btn, gpointer user_data);

/**
 * \brief Opens the camera movement configuration dialog.
 * \param btn The button that triggered the callback.
 * \param user_data The pointer tot the current application data.
 */
void on_validate_movement(GtkButton *btn, gpointer user_data);

/**
 * \brief Stores the parameters of a translation camera movement.
 * \param app Current application data.
 * \param dir_x Translation direction x component.
 * \param dir_y Translation direction y component.
 * \param dir_z Translation direction z component.
 * \param speed Translation speed.
 */
void on_validate_translation(AppData *app, float dir_x,float dir_y,float dir_z,float speed);

/**
 * \brief Stores the parameters of an orbit camera movement.
 * \param app Current application data.
 * \param center_x Orbit center x coordinate.
 * \param center_y Orbit center y coordinate.
 * \param center_z Orbit center z coordinate.
 * \param radius Orbit radius.
 * \param speed Angular speed.
 * \param elevation Orbit elevation angle.
 */
void on_validate_orbit(AppData *app, float center_x, float center_y, float center_z, float radius, float speed, float elevation);

/**
 * \brief Stores the parameters of a fly camera movement.
 * \param app Current application data.
 * \param target_x Target x coordinate.
 * \param target_y Target y coordinate.
 * \param target_z Target z coordinate.
 * \param speed Fly speed.
 * \param turn_rate Camera turn rate.
 */
void on_validate_fly(AppData *app, float target_x, float target_y, float target_z, float speed, float turn_rate);

/**
 * \brief Stores the parameters of a pendulum camera movement.
 * \param app Current application data.
 * \param pivot_x Pivot x coordinate.
 * \param pivot_y Pivot y coordinate.
 * \param pivot_z Pivot z coordinate.
 * \param radius Pendulum radius.
 * \param amplitude Pendulum amplitude.
 * \param frequency Pendulum frequency.
 */
void on_validate_pendulum(AppData *app, float pivot_x, float pivot_y, float pivot_z, float radius, float amplitude, float frequency);

/**
 * \brief Displays an error dialog window.
 * \param parent Parent gtk window.
 * \param message Error message to display.
 */
void show_error_dialog(GtkWindow *parent, const char *message);

/**
 * \brief Converts a string to a float while checking its validity.
 * \param text String to convert.
 * \param out Pointer receiving the converted value.
 * \return 1 if conversion succeeded, 0 otherwise.
 */
int parse_float_safe(const char *text, float *out);

/**
 * \brief Converts a string to an int while checking its validity.
 * \param text String to convert.
 * \param out Pointer receiving the converted value.
 * \return 1 if conversion succeeded, 0 otherwise.
 */
int parse_int_safe(const char *text, int *out);

#endif