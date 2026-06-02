/**
 * \file app.c
 * \author Nell TELECHEA
 * \brief Implementing app.h
 */ 

#include "app.h"
#include "controller.h"
#include "options.h"
#include "gtk_particle_renderer.h"

static GtkWidget *create_label_entry(char *label_text, GtkWidget **out_entry){
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);
	GtkWidget *label = gtk_label_new(label_text);
	GtkWidget *entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(box),entry,TRUE,TRUE,0);

	*out_entry = entry;
	return box;
}

void open_camera_option(AppData *data,int move){
	GtkWidget *dialog = gtk_dialog_new_with_buttons("Options camera",GTK_WINDOW(data->window),GTK_DIALOG_MODAL,"Valider",GTK_RESPONSE_ACCEPT,"Annuler",GTK_RESPONSE_CANCEL,NULL);
	GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	switch(move){
		case 1: 
            GtkWidget *entry_dir_x;
            GtkWidget *entry_dir_y;
            GtkWidget *entry_dir_z;
            GtkWidget *entry_speed;

            GtkWidget *box_dir_x = create_label_entry("Direction X :", &entry_dir_x);
            GtkWidget *box_dir_y = create_label_entry("Direction Y :", &entry_dir_y);
            GtkWidget *box_dir_z = create_label_entry("Direction Z :", &entry_dir_z);
            GtkWidget *box_speed = create_label_entry("Vitesse :", &entry_speed);

            gtk_box_pack_start(GTK_BOX(content),box_dir_x,FALSE, FALSE, 5);
            gtk_box_pack_start(GTK_BOX(content),box_dir_y,FALSE, FALSE, 5);
            gtk_box_pack_start(GTK_BOX(content),box_dir_z,FALSE, FALSE, 5);
            gtk_box_pack_start(GTK_BOX(content),box_speed,FALSE, FALSE, 5);

            gtk_widget_show_all(dialog);
            gint response = gtk_dialog_run(GTK_DIALOG(dialog));

            if(response == GTK_RESPONSE_ACCEPT){
				float x, y, z, speed;

			    if (!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_dir_x)), &x) ||
			        !parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_dir_y)), &y) ||
			        !parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_dir_z)), &z) ||
			        !parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_speed)), &speed)){
			        show_error_dialog(GTK_WINDOW(data->window),"Entrée invalide (valeurs numériques attendues)");
				    gtk_widget_destroy(dialog);
			        return;
			    }
			    on_validate_translation(data, x, y, z, speed);
            }
            break;
		
		case 2 :
			GtkWidget *entry_center_x;
			GtkWidget *entry_center_y;
			GtkWidget *entry_center_z;
			GtkWidget *entry_radius;
			GtkWidget *entry_speed_ang;
			GtkWidget *entry_elevation;

			GtkWidget *box_center_x = create_label_entry("Centre X :", &entry_center_x);
			GtkWidget *box_center_y = create_label_entry("Centre Y :", &entry_center_y);
			GtkWidget *box_center_z = create_label_entry("Centre Z :", &entry_center_z);
			GtkWidget *box_radius   = create_label_entry("Rayon :", &entry_radius);
			GtkWidget *box_speed_ang = create_label_entry("Vitesse angulaire :", &entry_speed_ang);
			GtkWidget *box_elevation = create_label_entry("Elevation :", &entry_elevation);

			gtk_box_pack_start(GTK_BOX(content), box_center_x, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_center_y, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_center_z, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_radius, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_speed_ang, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_elevation, FALSE, FALSE, 5);

			gtk_widget_show_all(dialog);
			gint response_orbit = gtk_dialog_run(GTK_DIALOG(dialog));

			if (response_orbit == GTK_RESPONSE_ACCEPT) {
				float cx, cy, cz, r, w, e;

				if (!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_center_x)), &cx) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_center_y)), &cy) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_center_z)), &cz) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_radius)), &r) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_speed_ang)), &w) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_elevation)), &e)){
					show_error_dialog(GTK_WINDOW(data->window),"Entrée invalide (valeurs numériques attendues)");
					gtk_widget_destroy(dialog);
					return;
				}
				on_validate_orbit(data, cx, cy, cz, r, w, e);
			}
			break;
		
		case 3:
			GtkWidget *entry_target_x;
			GtkWidget *entry_target_y;
			GtkWidget *entry_target_z;
			GtkWidget *entry_speed_fly;
			GtkWidget *entry_turn_rate;

			GtkWidget *box_target_x = create_label_entry("Cible X :", &entry_target_x);
			GtkWidget *box_target_y = create_label_entry("Cible Y :", &entry_target_y);
			GtkWidget *box_target_z = create_label_entry("Cible Z :", &entry_target_z);
			GtkWidget *box_speed_fly = create_label_entry("Vitesse :", &entry_speed_fly);
			GtkWidget *box_turn = create_label_entry("Taux rotation :", &entry_turn_rate);

			gtk_box_pack_start(GTK_BOX(content), box_target_x, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_target_y, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_target_z, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_speed_fly, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_turn, FALSE, FALSE, 5);

			gtk_widget_show_all(dialog);
			gint response_fly = gtk_dialog_run(GTK_DIALOG(dialog));

			if (response_fly == GTK_RESPONSE_ACCEPT) {
				float tx, ty, tz, sp, tr;

				if (!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_target_x)), &tx) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_target_y)), &ty) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_target_z)), &tz) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_speed_fly)), &sp) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_turn_rate)), &tr)){
					show_error_dialog(GTK_WINDOW(data->window),"Entrée invalide (valeurs numériques attendues)");
					gtk_widget_destroy(dialog);
					return;
				}
				on_validate_fly(data, tx, ty, tz, sp, tr);
			}
			break;

		case 4:
			GtkWidget *entry_pivot_x;
			GtkWidget *entry_pivot_y;
			GtkWidget *entry_pivot_z;
			GtkWidget *entry_radius_pendulum;
			GtkWidget *entry_amplitude;
			GtkWidget *entry_frequency;

			GtkWidget *box_pivot_x = create_label_entry("Pivot X :", &entry_pivot_x);
			GtkWidget *box_pivot_y = create_label_entry("Pivot Y :", &entry_pivot_y);
			GtkWidget *box_pivot_z = create_label_entry("Pivot Z :", &entry_pivot_z);
			GtkWidget *box_radius_pendulum  = create_label_entry("Rayon :", &entry_radius_pendulum);
			GtkWidget *box_amplitude = create_label_entry("Amplitude :", &entry_amplitude);
			GtkWidget *box_frequency = create_label_entry("Frequence :", &entry_frequency);

			gtk_box_pack_start(GTK_BOX(content), box_pivot_x, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_pivot_y, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_pivot_z, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_radius_pendulum, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_amplitude, FALSE, FALSE, 5);
			gtk_box_pack_start(GTK_BOX(content), box_frequency, FALSE, FALSE, 5);

			gtk_widget_show_all(dialog);
			gint response_pendulum = gtk_dialog_run(GTK_DIALOG(dialog));

			if (response_pendulum == GTK_RESPONSE_ACCEPT) {
				float px, py, pz, r, a, f;

				if (!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_pivot_x)), &px) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_pivot_y)), &py) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_pivot_z)), &pz) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_radius_pendulum)), &r) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_amplitude)), &a) ||
					!parse_float_safe(gtk_entry_get_text(GTK_ENTRY(entry_frequency)), &f))				{
					show_error_dialog(GTK_WINDOW(data->window),"Entrée invalide (valeurs numériques attendues)");
					gtk_widget_destroy(dialog);
					return;
				}
				on_validate_pendulum(data, px, py, pz, r, a, f);
			}
			break;

        default:
            show_error_dialog(GTK_WINDOW(data->window),"Entrez un mouvement entre 0 et 4");
            break;
    }
    gtk_widget_destroy(dialog);
}

AppData *app_create(GtkApplication *gtk_app){
    AppData *data = g_malloc(sizeof(AppData));

	//options struct
	data->opts = init_empty_options();

    //env struct
    data->environnement = NULL;
	data->camera_values_sum = 0;

	//window
    data->window = gtk_application_window_new(gtk_app);
    gtk_window_set_title(GTK_WINDOW(data->window),"Simulation particules");
    gtk_window_set_default_size(GTK_WINDOW(data->window),1800,900);

    // main layout
    data->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(data->window), data->main_box);

	data->control_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);

    // buttons + cam movement
	GtkWidget *actions_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    data->btn_next_iter = gtk_button_new_with_label("Next iteration");
	data->btn_create = gtk_button_new_with_label("Create envrionnement");
	GtkWidget *cam_movement_box = create_label_entry("Camera mouvement:",&data->entry_cam_movement);
	data->btn_validate_cam_m = gtk_button_new_with_label("Valider mouvement");
	gtk_box_pack_start(GTK_BOX(actions_group),data->btn_next_iter,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(actions_group),data->btn_create,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(actions_group),cam_movement_box,FALSE,FALSE,2);
	gtk_box_pack_start(GTK_BOX(actions_group),data->btn_validate_cam_m,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(data->control_bar), actions_group, FALSE, FALSE, 10);

	// param fields
	GtkWidget *sim_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *iter_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *cam_group  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	/* ----- Obstacle Group (added for Lot H.4) ----- */
	GtkWidget *obs_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_set_homogeneous(GTK_BOX(obs_group), FALSE);

	/* Title for obstacle section */
	GtkWidget *obs_title = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(obs_title), "<b>Obstacles</b>");

	/* Obstacle type selector (dropdown) */
	GtkWidget *obs_type_label = gtk_label_new("Type d'obstacle:");
	data->obs_type_combo = gtk_combo_box_text_new();
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(data->obs_type_combo), "Sphere");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(data->obs_type_combo), "Plane");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(data->obs_type_combo), "Box");
	gtk_combo_box_set_active(GTK_COMBO_BOX(data->obs_type_combo), 0);

	/* Position fields (common to all obstacle types) */
	GtkWidget *obs_x_box = create_label_entry("Position X:", &data->entry_obs_x);
	GtkWidget *obs_y_box = create_label_entry("Position Y:", &data->entry_obs_y);
	GtkWidget *obs_z_box = create_label_entry("Position Z:", &data->entry_obs_z);

	/* Sphere-specific fields */
	GtkWidget *obs_radius_label = gtk_label_new("Sphere parameters:");
	GtkWidget *obs_radius_box = create_label_entry("  Rayon:", &data->entry_obs_radius);

	/* Plane-specific fields */
	GtkWidget *obs_normal_label = gtk_label_new("Plane parameters:");
	GtkWidget *obs_nx_box = create_label_entry("  Normale X:", &data->entry_obs_nx);
	GtkWidget *obs_ny_box = create_label_entry("  Normale Y:", &data->entry_obs_ny);
	GtkWidget *obs_nz_box = create_label_entry("  Normale Z:", &data->entry_obs_nz);

	/* Box-specific fields */
	GtkWidget *obs_halfsize_label = gtk_label_new("Box parameters:");
	GtkWidget *obs_hx_box = create_label_entry("  Demi-largeur X:", &data->entry_obs_hx);
	GtkWidget *obs_hy_box = create_label_entry("  Demi-largeur Y:", &data->entry_obs_hy);
	GtkWidget *obs_hz_box = create_label_entry("  Demi-largeur Z:", &data->entry_obs_hz);

	/* Common fields */
	GtkWidget *obs_restitution_box = create_label_entry("Restitution (0=sticky, 1=elastic):", &data->entry_obs_restitution);

	/* Set default values for convenience */
	gtk_entry_set_text(GTK_ENTRY(data->entry_obs_x), "50");
	gtk_entry_set_text(GTK_ENTRY(data->entry_obs_y), "50");
	gtk_entry_set_text(GTK_ENTRY(data->entry_obs_z), "50");
	gtk_entry_set_text(GTK_ENTRY(data->entry_obs_radius), "15");
	gtk_entry_set_text(GTK_ENTRY(data->entry_obs_nx), "0");
	gtk_entry_set_text(GTK_ENTRY(data->entry_obs_ny), "1");
	gtk_entry_set_text(GTK_ENTRY(data->entry_obs_nz), "0");
	gtk_entry_set_text(GTK_ENTRY(data->entry_obs_hx), "20");
	gtk_entry_set_text(GTK_ENTRY(data->entry_obs_hy), "10");
	gtk_entry_set_text(GTK_ENTRY(data->entry_obs_hz), "10");
	gtk_entry_set_text(GTK_ENTRY(data->entry_obs_restitution), "0.8");

	/* Add obstacle button */
	data->btn_add_obstacle = gtk_button_new_with_label("➕ Ajouter obstacle");

	/* Pack all widgets into obstacle group */
	gtk_box_pack_start(GTK_BOX(obs_group), obs_title, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(obs_group), obs_type_label, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), data->obs_type_combo, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), obs_x_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), obs_y_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), obs_z_box, FALSE, FALSE, 2);

	gtk_box_pack_start(GTK_BOX(obs_group), obs_radius_label, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), obs_radius_box, FALSE, FALSE, 2);

	gtk_box_pack_start(GTK_BOX(obs_group), obs_normal_label, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), obs_nx_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), obs_ny_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), obs_nz_box, FALSE, FALSE, 2);

	gtk_box_pack_start(GTK_BOX(obs_group), obs_halfsize_label, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), obs_hx_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), obs_hy_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), obs_hz_box, FALSE, FALSE, 2);

	gtk_box_pack_start(GTK_BOX(obs_group), obs_restitution_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(obs_group), data->btn_add_obstacle, FALSE, FALSE, 5);

	/* Add obstacle group to control bar */
	gtk_box_pack_start(GTK_BOX(data->control_bar), obs_group, FALSE, FALSE, 10);

	gtk_box_pack_start(GTK_BOX(data->control_bar), sim_group, FALSE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(data->control_bar), iter_group, FALSE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(data->control_bar), cam_group, FALSE, FALSE, 10);

	GtkWidget *p_nb_box = create_label_entry("Nombre particules:",&data->entry_particle_nb);
	GtkWidget *w_box = create_label_entry("Largeur:",&data->entry_width);
	GtkWidget *h_box = create_label_entry("Hauteur:",&data->entry_height);
	GtkWidget *d_box = create_label_entry("Profondeur:",&data->entry_depth);
	GtkWidget *r_box = create_label_entry("Rayon iteration:",&data->entry_r);
	GtkWidget *tps_inter_box = create_label_entry("Durée iteration:",&data->entry_iter_t);
	GtkWidget *x_cam_box = create_label_entry("X camera:",&data->entry_cam_x);
	GtkWidget *y_cam_box = create_label_entry("Y camera:",&data->entry_cam_y);
	GtkWidget *z_cam_box = create_label_entry("Z camera:",&data->entry_cam_z);
	GtkWidget *x_dir_cam_box = create_label_entry("X direction camera:",&data->entry_cam_dir_x);
	GtkWidget *y_dir_cam_box = create_label_entry("Y direction camera:",&data->entry_cam_dir_y);
	GtkWidget *z_dir_cam_box = create_label_entry("Z direction camera:",&data->entry_cam_dir_z);

	gtk_box_pack_start(GTK_BOX(sim_group), p_nb_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(sim_group), w_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(sim_group), h_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(sim_group), d_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(iter_group), r_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(iter_group), tps_inter_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), x_cam_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), y_cam_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), z_cam_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), x_dir_cam_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), y_dir_cam_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), z_dir_cam_box, FALSE, FALSE, 2);


	gtk_box_pack_start(GTK_BOX(data->main_box),data->control_bar,FALSE,FALSE,5);

	//rendu
	data->render_area = gtk_drawing_area_new();
    g_signal_connect(data->render_area, "draw", G_CALLBACK(particle_draw), data);
	gtk_box_pack_start(GTK_BOX(data->main_box),data->render_area,TRUE,TRUE,5);

	//controller handling
    g_signal_connect(data->btn_next_iter,"clicked",G_CALLBACK(on_next),data);
	g_signal_connect(data->btn_create,"clicked",G_CALLBACK(on_create),data);
	g_signal_connect(data->btn_validate_cam_m,"clicked",G_CALLBACK(on_validate_movement),data);
	g_signal_connect(data->btn_add_obstacle, "clicked", G_CALLBACK(on_add_obstacle), data);

	gtk_widget_show_all(data->window);

    return data;
}