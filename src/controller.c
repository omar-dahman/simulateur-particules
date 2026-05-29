#include <stdio.h>
#include <stdlib.h>
#include "controller.h"
#include "app.h"

static void sync_parameters_environment(AppData *data){
    const char *p_nb_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_particle_nb));
	const char *w_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_width));
	const char *h_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_height));
	const char *d_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_depth));
	const char *r_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_r));
	const char *nb_iter_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_iter_nb));
	const char *tps_iter_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_iter_t));
}

static void sync_parameters_camera(AppData *data){
	const char *x_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_x));
	const char *y_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_y));
	const char *z_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_z));
	const char *x_dir_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_x));
	const char *y_dir_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_y));
	const char *z_dir_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_z));

	GtkAllocation allocation;
	gtk_widget_get_allocation(data->render_area,&allocation);
	const int width_render_area = allocation.width;
	const int height_render_area = allocation.height;
	printf("%d %d\n",width_render_area, height_render_area);
}

void on_next(GtkButton *btn, gpointer user_data){
    (void)btn;
    AppData *app = (AppData *)user_data;

	sync_parameters_camera(app);

    printf("Iteration avancée\n");
}

void on_create(GtkButton *btn, gpointer user_data){
	(void)btn;
	AppData *app = (AppData *)user_data;
	
	sync_parameters_environment(app);
    printf("Reset environnement\n");
}