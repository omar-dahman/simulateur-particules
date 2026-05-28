#include <gtk/gtk.h>
#include "app.h"

static void activate(GtkApplication *gtk_app, gpointer user_data){
    (void)user_data;

    app_create(gtk_app);

}

int main(int argc, char **argv){
    GtkApplication *app;
    int status;

    app = gtk_application_new("sim.particule", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}