/**
 * \file controller.c
 * \author Nell TELECHEA
 * \brief Implementing controller.h
 */

#include <stdio.h>
#include <stdlib.h>
#include "controller.h"
#include "app.h"
#include "options.h"
#include "environnement.h"
#include "camera.h"
#include "vector.h"

int parse_float_safe(const char *text, float *out){
	if(text == NULL){
		return 0;
	}
    char *endptr;
    float value = strtof(text, &endptr);

    if (endptr == text || *endptr != '\0'){
        return 0;
	}
	if (value < 0.0) {
		return 0;
	}
    *out = value;
    return 1;
}

int parse_int_safe(const char *text, int *out){
	if(text == NULL){
		return 0;
	}
    char *endptr;
    int value = (int) strtol(text, &endptr, 0);

    if (endptr == text || *endptr != '\0'){
        return 0;
	}
	if (value <= 0) {
		return 0;
	}
    *out = value;
    return 1;
}

static int sync_parameters_environment(AppData *data){
	options opts = data->opts;

	if (!parse_int_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_particle_nb)), &opts->particle_nb) ||
		!parse_int_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_width)), &opts->w) ||
		!parse_int_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_height)), &opts->h) ||
		!parse_int_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_depth)), &opts->d) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_r)), &opts->radius) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_iter_t)), &opts->iteration_t)){
		show_error_dialog(GTK_WINDOW(data->window),"Entrée invalide (valeurs numériques attendues) ou vide");
		return -1;
	}
	return 0;
}

/**
 * @return -1 for errors, 0 for no change, 1 for change against old values or on initialization
 */
static int sync_parameters_camera(AppData *data){
	options opts = data->opts;

	float sum = opts->cam_dir_x + opts->cam_dir_y + opts->cam_dir_y;
	
	if (!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_x)), &opts->cam_x) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_y)), &opts->cam_y) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_z)), &opts->cam_z) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_x)), &opts->cam_dir_x) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_y)), &opts->cam_dir_y) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_z)), &opts->cam_dir_z)){
		show_error_dialog(GTK_WINDOW(data->window),"Entrée invalide (valeurs numériques attendues) ou vide");
		return -1;
	}
	if (sum < 1e-6f || (sum - data->camera_values_sum) > 1e-6f) { // check for initialization
		data->camera_values_sum = sum;
		return 1;
	}
	return 0;
}

void on_next(GtkButton *btn, gpointer user_data){
    (void)btn;
    AppData *app = (AppData *)user_data;

	int sync = sync_parameters_camera(app);

	if(sync !=-1 && app->opts->w != 0){
		// Check the env has been initialized
		move_particules(app->environnement);
		if (sync==1) {
			options opts = app->opts;
			vec3 pos = vec3_make(opts->cam_x, opts->cam_y, opts->cam_z);
			vec3 dir = vec3_make(opts->cam_dir_x, opts->cam_dir_y, opts->cam_dir_z);
			app->cam = camera_create(pos, dir, opts->img_w, opts->img_h);
		} else {
			camera_update(&app->cam);
		}
		gtk_widget_queue_draw(app->render_area);
	}else{
		//do nothing
	}
}

void on_create(GtkButton *btn, gpointer user_data){
	(void)btn;
	AppData *app = (AppData *)user_data;
	
	int sync_env = sync_parameters_environment(app);
	int sync_cam = sync_parameters_camera(app);
	
	if(sync_env !=-1 && sync_cam!=-1){
		options opts = app->opts;
		app->environnement = create_environnement(opts->particle_nb, opts->w, opts->h, opts->d, opts->radius, opts->iteration_t);
		gtk_widget_queue_draw(app->render_area);
	}else{
		//do nothing
	}
}

void on_validate_movement(GtkButton *btn, gpointer user_data){
	(void)btn;
	AppData *app = (AppData *)user_data;
	const char *movement_cam_text  = gtk_entry_get_text(GTK_ENTRY(app->entry_cam_movement));
	int movement = atoi(movement_cam_text);
	if(movement!=0){
		open_camera_option(app,movement);
	} else {
		camera_set_move_none(&app->cam);
	}
}

void on_validate_translation(AppData *app, float dir_x,float dir_y,float dir_z,float speed) {
	options opts = app->opts;
	vec3 dir = vec3_make(dir_x, dir_y, dir_z);
	opts->camera_move_type=1;
	opts->trans_dir_x = dir_x;
	opts->trans_dir_y = dir_y;
	opts->trans_dir_z = dir_z;
	opts->trans_speed = speed;
	camera_set_move_translate(&app->cam, dir, speed);
}

void on_validate_orbit(AppData *app, float center_x, float center_y, float center_z, float radius, float speed, float elevation){
	options opts = app->opts;
	vec3 center = vec3_make(center_x, center_y, center_z);
	opts->camera_move_type = 2;
	opts->orbit_center_x = center_x;
	opts->orbit_center_y = center_y;
	opts->orbit_center_z = center_z;
	opts->orbit_radius = radius;
	opts->orbit_angular_speed = speed;
	opts->orbit_elevation = elevation;
	camera_set_move_orbit(&app->cam, center, radius, speed, elevation);
}

void on_validate_fly(AppData *app, float target_x, float target_y, float target_z, float speed, float turn_rate){
	options opts = app->opts;
	vec3 target = vec3_make(target_x, target_y, target_z);
	opts->camera_move_type=3;
	opts->fly_target_x = target_x;
	opts->fly_target_y = target_y;
	opts->fly_target_z = target_z;
	opts->fly_speed = speed;
	opts->fly_turn_rate = turn_rate;
	camera_set_move_fly(&app->cam, target, speed, turn_rate);
}

void on_validate_pendulum(AppData *app, float pivot_x, float pivot_y, float pivot_z, float radius, float amplitude, float frequency){
	options opts = app->opts;
	vec3 pivot = vec3_make(pivot_x, pivot_y, pivot_y);
	opts->camera_move_type=4;
	opts->pend_pivot_x = pivot_x;
	opts->pend_pivot_y = pivot_y;
	opts->pend_pivot_z = pivot_z;
	opts->pend_radius = radius;
	opts->pend_amplitude = amplitude;
	opts->pend_frequency = frequency;
	camera_set_move_pendulum(&app->cam, pivot, radius, amplitude, frequency);
}

void show_error_dialog(GtkWindow *parent, const char *msg){
    GtkWidget *d = gtk_message_dialog_new(parent,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"%s",msg);
    gtk_dialog_run(GTK_DIALOG(d));
    gtk_widget_destroy(d);
}

