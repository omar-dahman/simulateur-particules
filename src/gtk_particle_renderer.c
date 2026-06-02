#include "gtk/gtk.h"
#include "cairo.h"
#include <math.h>

#include "camera.h"
#include "interface.h"
#include "app.h"
#include "environnement.h"
#include "vector.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define REDUCING_FACTOR 3

gboolean particle_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    short *render;
    AppData *app = (AppData *)user_data;
    int width = gtk_widget_get_allocated_width(widget) / REDUCING_FACTOR;
    int height = gtk_widget_get_allocated_height(widget) / REDUCING_FACTOR;

    if (app->environnement == NULL)
    {
        return FALSE;
    }

    camera_size_update(&app->cam, width, height);

    /* Clear background to black */
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    /* Draw particles in white */
    cairo_set_source_rgb(cr, 1, 1, 1);
    render = render_3d(height, width, app->environnement, &app->cam);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            short pixel = render[j * width + i];
            if (pixel == 1)
            {
                cairo_rectangle(cr, i * REDUCING_FACTOR, j * REDUCING_FACTOR,
                                REDUCING_FACTOR, REDUCING_FACTOR);
                cairo_fill(cr);
            }
        }
    }
    free(render);

    /* Draw obstacles using getter functions (added for Lot H.4) */
    int obstacle_count = get_obstacle_count(app->environnement);
    if (obstacle_count > 0)
    {
        float w_env = get_w(app->environnement);
        float h_env = get_h(app->environnement);

        for (int i = 0; i < obstacle_count; i++)
        {
            const Obstacle *obs = get_obstacle(app->environnement, i);
            if (obs == NULL)
                continue;

            /* Project obstacle position to screen coordinates */
            int screen_x = (int)(obs->position.x * width / w_env);
            int screen_y = (int)(obs->position.y * height / h_env);

            cairo_set_source_rgb(cr, 0.6, 0.6, 0.6); /* Light gray for obstacles */

            switch (obs->type)
            {
            case OBSTACLE_SPHERE:
            {
                /* Draw a filled circle */
                int radius_px = (int)(obs->radius * width / w_env);
                if (radius_px < 2)
                    radius_px = 2;
                cairo_arc(cr,
                          screen_x * REDUCING_FACTOR,
                          screen_y * REDUCING_FACTOR,
                          radius_px * REDUCING_FACTOR,
                          0, 2 * M_PI);
                cairo_fill(cr);

                /* Draw outline */
                cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
                cairo_arc(cr,
                          screen_x * REDUCING_FACTOR,
                          screen_y * REDUCING_FACTOR,
                          radius_px * REDUCING_FACTOR,
                          0, 2 * M_PI);
                cairo_stroke(cr);
                break;
            }

            case OBSTACLE_PLANE:
            {
                /* Draw a semi-transparent line representing the plane */
                cairo_set_source_rgba(cr, 0.5, 0.5, 0.8, 0.5);
                cairo_move_to(cr, 0, screen_y * REDUCING_FACTOR);
                cairo_line_to(cr, width * REDUCING_FACTOR, screen_y * REDUCING_FACTOR);
                cairo_stroke(cr);
                break;
            }

            case OBSTACLE_BOX:
            {
                /* Draw a filled rectangle */
                int half_w = (int)(obs->half_size.x * width / w_env);
                int half_h = (int)(obs->half_size.y * height / h_env);
                if (half_w < 1)
                    half_w = 1;
                if (half_h < 1)
                    half_h = 1;

                cairo_rectangle(cr,
                                (screen_x - half_w) * REDUCING_FACTOR,
                                (screen_y - half_h) * REDUCING_FACTOR,
                                half_w * 2 * REDUCING_FACTOR,
                                half_h * 2 * REDUCING_FACTOR);
                cairo_fill(cr);

                /* Draw outline */
                cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
                cairo_rectangle(cr,
                                (screen_x - half_w) * REDUCING_FACTOR,
                                (screen_y - half_h) * REDUCING_FACTOR,
                                half_w * 2 * REDUCING_FACTOR,
                                half_h * 2 * REDUCING_FACTOR);
                cairo_stroke(cr);
                break;
            }
            }
        }

        /* Restore color for future drawings */
        cairo_set_source_rgb(cr, 1, 1, 1);
    }

    return TRUE;
}