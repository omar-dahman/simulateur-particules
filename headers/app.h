#ifndef APP_H
#define APP_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *entry_r;
	GtkWidget *entry_width, *entry_height, *entry_depth, *entry_particle_nb, *entry_iter_nb, *entry_iter_t, *entry_w_img, *entry_h_img;
	GtkWidget *entry_cam_x, *entry_cam_y, *entry_cam_z, *entry_cam_dir_x, *entry_cam_dir_y, *entry_cam_dir_z;
	GtkWidget *control_bar;
    GtkWidget *btn_next_iter;
    GtkWidget *btn_reset;
	GtkWidget *render_area;
} AppData;

AppData *app_create(GtkApplication *app);

#endif