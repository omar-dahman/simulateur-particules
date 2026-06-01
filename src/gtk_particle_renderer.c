#include "gtk/gtk.h"
#include "cairo.h"

#include "camera.h"
#include "interface.h"
#include "app.h"

#define REDUCING_FACTOR 3

gboolean particle_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    short* render;
    AppData *app = (AppData *)user_data;
    options opts = app->opts;
    int width = gtk_widget_get_allocated_width(widget) / REDUCING_FACTOR;
    int height = gtk_widget_get_allocated_height(widget)/ REDUCING_FACTOR;
    camera_size_update(&app->cam, width, height);
    if (app->environnement == NULL) {
        return FALSE;
    }

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);
    
    cairo_set_source_rgb(cr, 1, 1, 1);
    render = render_3d(height, width, app->environnement, &app->cam);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            short pixel = render[j*width+i];
            if (pixel == 1) {
                cairo_rectangle(cr, i * REDUCING_FACTOR, j * REDUCING_FACTOR, REDUCING_FACTOR, REDUCING_FACTOR);
                cairo_fill(cr);
            }
        }
    }
    free(render);

    return TRUE;
}

