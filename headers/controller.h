#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <gtk/gtk.h>
#include "app.h"

void on_next(GtkButton *btn, gpointer user_data);
void on_create(GtkButton *btn, gpointer user_data);
void on_validate_movement(GtkButton *btn, gpointer user_data);
void on_validate_translation(AppData *app, float dir_x,float dir_y,float dir_z,float speed);
void on_validate_orbit(AppData *app, float center_x, float center_y, float center_z, float radius, float speed, float elevation);
void on_validate_fly(AppData *app, float target_x, float target_y, float target_z, float speed, float turn_rate);
void on_validate_pendulum(AppData *app, float pivot_x, float pivot_y, float pivot_z, float radius, float amplitude, float frequency);
void show_error_dialog(GtkWindow *parent, const char *message);
int parse_float_safe(const char *text, float *out);
int parse_int_safe(const char *text, int *out);

#endif