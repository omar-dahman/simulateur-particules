#include <stdio.h>
#include <stdlib.h>
#include "controller.h"
#include "app.h"

static void sync_parameters(AppData *data){
    const char *p_nb_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_particle_nb));
	const char *w_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_width));
	const char *h_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_height));
	const char *d_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_depth));
	const char *r_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_r));
	const char *nb_iter_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_iter_nb));
	const char *tps_iter_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_iter_t));
	const char *w_i_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_w_img));
	const char *h_i_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_h_img));
	const char *x_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_x));
	const char *y_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_y));
	const char *z_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_z));
	const char *x_dir_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_x));
	const char *y_dir_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_y));
	const char *z_dir_cam_text  = gtk_entry_get_text(GTK_ENTRY(data->entry_cam_dir_z));

}

void on_next(GtkButton *btn, gpointer user_data){
    (void)btn;
    AppData *app = (AppData *)user_data;

	sync_parameters(app);

    printf("Iteration avancée\n");
}

void on_reset(GtkButton *btn, gpointer user_data){
	(void)btn;
	AppData *app = (AppData *)user_data;
	
	gtk_entry_set_text(GTK_ENTRY(app->entry_particle_nb), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_width), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_height), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_depth), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_r), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_iter_nb), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_iter_t), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_w_img), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_h_img), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_cam_x), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_cam_y), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_cam_z), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_cam_dir_x), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_cam_dir_y), "");
	gtk_entry_set_text(GTK_ENTRY(app->entry_cam_dir_z), "");

    printf("Reset environnement\n");
}