#include "app.h"
#include "controller.h"

static GtkWidget *create_label_entry(char *label_text, GtkWidget **out_entry){
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);
	GtkWidget *label = gtk_label_new(label_text);
	GtkWidget *entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(box),entry,TRUE,TRUE,0);

	*out_entry = entry;
	return box;
}

AppData *app_create(GtkApplication *gtk_app){
    AppData *data = g_malloc(sizeof(AppData));

	//window
    data->window = gtk_application_window_new(gtk_app);
    gtk_window_set_title(GTK_WINDOW(data->window),"Simulation particules");
    gtk_window_set_default_size(GTK_WINDOW(data->window),1800,900);

    // main layout
    data->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(data->window), data->main_box);

	data->control_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);

    // buttons
	GtkWidget *actions_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    data->btn_next_iter = gtk_button_new_with_label("Next iteration");
	data->btn_reset = gtk_button_new_with_label("Reset envrionnement");
	gtk_box_pack_start(GTK_BOX(actions_group),data->btn_next_iter,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(actions_group),data->btn_reset,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(data->control_bar), actions_group, FALSE, FALSE, 10);

	// param fields
	GtkWidget *sim_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *iter_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *img_group  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *cam_group  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(data->control_bar), sim_group, FALSE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(data->control_bar), iter_group, FALSE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(data->control_bar), img_group, FALSE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(data->control_bar), cam_group, FALSE, FALSE, 10);

	GtkWidget *p_nb_box = create_label_entry("Nombre particules:",&data->entry_particle_nb);
	GtkWidget *w_box = create_label_entry("Largeur:",&data->entry_width);
	GtkWidget *h_box = create_label_entry("Hauteur:",&data->entry_height);
	GtkWidget *d_box = create_label_entry("Profondeur:",&data->entry_depth);
	GtkWidget *r_box = create_label_entry("Rayon iteration:",&data->entry_r);
	GtkWidget *nb_inter_box = create_label_entry("Nombre iteration:",&data->entry_iter_nb);
	GtkWidget *tps_inter_box = create_label_entry("Durée iteration:",&data->entry_iter_t);
	GtkWidget *w_i_box = create_label_entry("Largeur image:",&data->entry_w_img);
	GtkWidget *h_i_box = create_label_entry("Hauteur image:",&data->entry_h_img);
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
	gtk_box_pack_start(GTK_BOX(iter_group), nb_inter_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(iter_group), tps_inter_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(img_group), w_i_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(img_group), h_i_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), x_cam_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), y_cam_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), z_cam_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), x_dir_cam_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), y_dir_cam_box, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(cam_group), z_dir_cam_box, FALSE, FALSE, 2);


	gtk_box_pack_start(GTK_BOX(data->main_box),data->control_bar,FALSE,FALSE,5);

	//rendu
	data->render_area = gtk_frame_new("Rendu");
	gtk_box_pack_start(GTK_BOX(data->main_box),data->render_area,TRUE,TRUE,5);

	//controller handling
    g_signal_connect(data->btn_next_iter,"clicked",G_CALLBACK(on_next),data);
	g_signal_connect(data->btn_reset,"clicked",G_CALLBACK(on_reset),data);
    
    gtk_widget_show_all(data->window);

    return data;
}