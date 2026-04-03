/**
 * \file    interface.c
 * \author  Omar DAHMAN
 * \date    2026-04-04
 *
 * \brief   User interface and I/O management (Implementation)
 *
 * \note MODIFIED FOR LOT E (Tâche E.2):
 *   - print_particule: now prints z-coordinate and vz.
 *   - print_environnement: now prints depth d.
 *   - is_pixel_occupied: uses 3D position but projects onto XY plane for PBM output.
 *   - snapshot / animate: unchanged in signature.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"
#include "environnement.h"
#include "particules.h"


void print_particule(particule p) {
    if (p == NULL) return;

    /* MODIFIED FOR LOT E (Tâche E.2): added z and vz */
    printf("Particule :\n");
    printf("  Position : (%f , %f , %f)\n", get_x(p), get_y(p), get_z(p));
    printf("  Vitesse  : (%f , %f , %f)\n", get_vx(p), get_vy(p), get_vz(p));
}

void print_environnement(env e) {
    if (e == NULL) return;

    /* MODIFIED FOR LOT E (Tâche E.2): added depth d */
    printf("Environnement :\n");
    printf("  Nombre de particules : %d\n", get_n(e));
    printf("  Largeur  (w) : %f\n", get_w(e));
    printf("  Hauteur  (h) : %f\n", get_h(e));
    printf("  Profondeur (d) : %f\n", get_d(e));   /* ADDED FOR LOT E (Tâche E.2) */
    printf("  dt       : %f\n", get_dt(e));
    printf("  Rayon R  : %f\n\n", get_r(e));

    for (int i = 0; i < get_n(e); i++) {
        printf("Particule %d :\n", i);
        print_particule(get_particule(e, i));
        printf("\n");
    }
}

void print_message(char *message) {
    if (message != NULL)
        printf("%s", message);
}


int get_int(void) {
    int value;
    int ret;
    do {
        ret = scanf("%d", &value);
        if (ret != 1) {
            printf("Entrée invalide. Veuillez entrer un entier : ");
            while (getchar() != '\n'); /* flush buffer */
        }
    } while (ret != 1);
    return value;
}

float get_float(void) {
    float value;
    int ret;
    do {
        ret = scanf("%f", &value);
        if (ret != 1) {
            printf("Entrée invalide. Veuillez entrer un float : ");
            while (getchar() != '\n'); /* flush buffer */
        }
    } while (ret != 1);
    return value;
}


/**
 * \brief   Test whether pixel (px, py) is occupied by any particle.
 * \details Projects 3D particle positions onto the XY plane to determine
 *          pixel occupancy for the PBM output.
 * \note MODIFIED FOR LOT E (Tâche E.2): get_x/get_y still used for XY
 *       projection; get_z is not needed for flat PBM (camera task handles 3D view).
 *
 * \param   e   environment
 * \param   px  pixel x-coordinate
 * \param   py  pixel y-coordinate
 * \param   wp  image width  in pixels
 * \param   hp  image height in pixels
 * \return  1 if occupied, 0 otherwise
 */
static int is_pixel_occupied(env e, int px, int py, int wp, int hp) {
    int n = get_n(e);
    for (int i = 0; i < n; i++) {
        particule p = get_particule(e, i);

        /* Project 3D position onto XY plane */
        int pixel_x = (int)(get_x(p) * wp / get_w(e));
        int pixel_y = (int)(get_y(p) * hp / get_h(e));

        if (pixel_x == px && pixel_y == py)
            return 1;
    }
    return 0;
}

void snapshot(env e, char *path, int wp, int hp) {
    FILE *file = fopen(path, "w");
    if (file == NULL) return;

    fprintf(file, "P1\n");
    fprintf(file, "# Snapshot environnement 3D (projection XY)\n");
    fprintf(file, "%d %d\n", wp, hp);

    for (int y = 0; y < hp; y++) {
        for (int x = 0; x < wp; x++) {
            fprintf(file, "%d ", is_pixel_occupied(e, x, y, wp, hp));
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void animate(env e, char *path, int wp, int hp, int t) {
    char filename[256];
    for (int i = 0; i < t; i++) {
        snprintf(filename, sizeof(filename), "%s/env-%d.pbm", path, i);
        snapshot(e, filename, wp, hp);
        move_particules(e);
    }
}
