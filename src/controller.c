#include <stdio.h>
#include <stdlib.h>
#include "controller.h"
#include "app.h"

static int sync_parameters_environment(AppData *data){
	float p_nb,w,h,d,r,tps_iter;

	if (!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_particle_nb)), &p_nb) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_width)), &w) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_height)), &h) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_depth)), &d) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_r)), &r) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_iter_t)), &tps_iter)){
		show_error_dialog(GTK_WINDOW(data->window),"Entrée invalide (valeurs numériques attendues) ou vide");
		return -1;
		}else{
			return 0;
		}
}

static int sync_parameters_camera(AppData *data){
	float cam_x,cam_y,cam_z,cam_dir_x,cam_dir_y,cam_dir_z;

	if (!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_x)), &cam_x) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_y)), &cam_y) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_z)), &cam_z) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_x)), &cam_dir_x) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_y)), &cam_dir_y) ||
		!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_z)), &cam_dir_z)){
		show_error_dialog(GTK_WINDOW(data->window),"Entrée invalide (valeurs numériques attendues) ou vide");
		return -1;
		}else{
			return 0;
		}

	GtkAllocation allocation;
	gtk_widget_get_allocation(data->render_area,&allocation);
	const int width_render_area = allocation.width;
	const int height_render_area = allocation.height;
	printf("%d %d\n",width_render_area, height_render_area);
}

void on_next(GtkButton *btn, gpointer user_data){
    (void)btn;
    AppData *app = (AppData *)user_data;

	int sync = sync_parameters_camera(app);

	if(sync !=-1){
		//ok
	}else{
		//do nothing
	}

    printf("Iteration avancée\n");
}

void on_create(GtkButton *btn, gpointer user_data){
	(void)btn;
	AppData *app = (AppData *)user_data;
	
	int sync = sync_parameters_environment(app);
	
	if(sync !=-1){
		//ok
	}else{
		//do nothing
	}
    printf("Create environnement\n");
}

void on_validate_movement(GtkButton *btn, gpointer user_data){
	(void)btn;
	AppData *app = (AppData *)user_data;
	const char *movement_cam_text  = gtk_entry_get_text(GTK_ENTRY(app->entry_cam_movement));
	int movement = atoi(movement_cam_text);
	printf("%d\n",movement);
	if(movement!=0){
		open_camera_option(app,movement);
	}
}

void on_validate_translation(float dir_x,float dir_y,float dir_z,float speed) {
	printf("Translation: %f %f %f %f\n", dir_x, dir_y, dir_z, speed);
}

void on_validate_orbit(float center_x, float center_y, float center_z, float radius, float speed, float elevation){
	printf("orbit\n");
}

void on_validate_fly(float target_x, float target_y, float target_z, float speed, float turn_rate){
	printf("fly\n");
}

void on_validate_pendulum(float pivot_x, float pivot_y, float pivot_z, float radius, float amplitude, float frequency){
	printf("pendulum\n");
}

void show_error_dialog(GtkWindow *parent, const char *msg){
    GtkWidget *d = gtk_message_dialog_new(parent,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"%s",msg);
    gtk_dialog_run(GTK_DIALOG(d));
    gtk_widget_destroy(d);
}

int parse_float_safe(const char *text, float *out){
	if(text == NULL){
		return 0;
	}
    char *endptr;
    float value = strtof(text, &endptr);

    if (endptr == text || *endptr != '\0'){
        return 0;
	}
    *out = value;
    return 1;
}
