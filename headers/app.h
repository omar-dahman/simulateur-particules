/**
 * \file app.h
 * \author Nell TELECHEA
 * \brief Graphical user interface definitions.
 * \details This file declares the AppData structure, which stores all GTK widgets used by the application, as well as functions responsible for creating and managing the gui.
 */
#ifndef APP_H
#define APP_H

#include <gtk/gtk.h>
#include "options.h"
#include "environnement.h"
#include "camera.h"

/**
 * \struct AppData
 * \brief Main structure containing the state of the graphical application.
 * \details Stores all GTK widgets used by the interface, as well as the
 *          simulation objects currently manipulated by the GUI
 *          (environment, camera and simulation options).
 */
typedef struct {
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *entry_r;
	GtkWidget *entry_width, *entry_height, *entry_depth, *entry_particle_nb, *entry_iter_t;
	GtkWidget *entry_cam_x, *entry_cam_y, *entry_cam_z, *entry_cam_dir_x, *entry_cam_dir_y, *entry_cam_dir_z;
	GtkWidget *control_bar;
    GtkWidget *btn_next_iter;
    GtkWidget *btn_create;
	GtkWidget *render_area;
    GtkWidget *entry_cam_movement;
    GtkWidget *btn_validate_cam_m;
    options opts;
    env environnement;
    Camera cam;
    float camera_values_sum;
} AppData;

/**
 * \brief Creates and initializes the graphical application.
 * \param app GTK application.
 * \return Pointer to the new allocated AppData structure.
 */
AppData *app_create(GtkApplication *app);

/**
 * \brief Opens a modal dialog used to configure camera movement options.
 * \param data Current application data.
 * \param move Camera movement type selected by the user.
 */
void open_camera_option(AppData *data,int move);

#endif