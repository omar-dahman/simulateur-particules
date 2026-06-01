#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "interface.h"
#include "environnement.h"
#include "particules.h"
#include "vector.h"
#include "camera.h"

#define FILENAME_BUFFER_LENGTH 256

/**
 * \file    interface.c
 *
 * \brief   User interface and I/O management (Implementation)
 *
 * \note MODIFIED FOR LOT E (Tâche E.2):
 *   - print_particule: now prints z-coordinate and vz.
 *   - print_environnement: now prints depth d.
 *   - is_pixel_occupied: uses 3D position but projects onto XY plane for PBM output.
 *   - snapshot / animate: unchanged in signature.
 * \note MODIFIED FOR LOT E (Tâche E.4):
 *   - render_2d : created
 *   - render_3d : created
 *   - animate : added camera argument
 *   - snapshot : added camera argument
 */


void print_particule(particule p) {
    if (p == NULL)
        return;

    /* MODIFIED FOR LOT E (Tâche E.2): added z and vz */
    printf("Particule :\n");
    printf("  Position : (%f , %f , %f)\n", get_x(p), get_y(p), get_z(p));
    printf("  Vitesse  : (%f , %f , %f)\n", get_vx(p), get_vy(p), get_vz(p));
}

void print_environnement(env e) {
    if (e == NULL)
        return;

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
            while (getchar() != '\n'); // flush buffer
        }
    } while (ret != 1);
    return value;
}

float get_float() {
    float value;
    int ret;
    do {
        ret = scanf("%f", &value);
        if (ret != 1) {
            printf("Entrée invalide. Veuillez entrer un float : ");
            while (getchar() != '\n'); // flush buffer
        }
    } while (ret != 1);
    return value;
}


// All below has been modified for tâche e.4

short* render_2d(int hd, int wd, env e) {
    short* render = calloc(hd * wd, sizeof (short));
    int n = get_n(e);
    float we = get_w(e);
    float wh = get_h(e);
    for(int i=0; i<n; i++) {
        particule p = get_particule(e, i);
        int x = (int)(get_x(p) * wd / we);
        int y = (int)(get_y(p) * hd / wh);
        render[x*wd+y] = 1;
    }
    return render;
}

short* render_3d(int hd, int wd, env e, Camera* cam) {
    short* render = calloc(hd * wd, sizeof (short));
    int n = get_n(e);
    for(int i=0; i<n; i++) {
        particule p = get_particule(e, i);
        int px, py;
        vec3 pos = vec3_make(get_x(p), get_y(p), get_z(p));
        if (camera_is_particle_visible(cam, pos, &px, &py)==1) {
            render[py * wd + px] = 1;
        }
    }
    return render;
}

void snapshot(env e, char *path, int wp, int hp, Camera* cam) {
    FILE *file = fopen(path, "w");
    short* render;
    if (file == NULL)
        return;
    fprintf(file, "P1\n");
    fprintf(file, "# Snapshot environnement\n");
    fprintf(file, "%d %d\n", wp, hp);

    if (cam==NULL) {
        render = render_2d(hp, wp, e);
    } else {
        render = render_3d(hp, wp, e, cam);
    }

    for (int x = 0; x < wp; x++) {
        for (int y = 0; y < hp; y++) {
            fprintf(file, "%d ", render[x*wp+y]);
        }
        fprintf(file, "\n");
    }
    free(render);
    fclose(file);
}

void animate(env e, char *path, int wp, int hp, int t, Camera* cam) {
    char filename[FILENAME_BUFFER_LENGTH];

    for (int i = 0; i < t; i++) {
        snprintf(filename, FILENAME_BUFFER_LENGTH, "%s/env-%d.pbm", path, i);
        snapshot(e, filename, wp, hp, cam);
        move_particules(e);
        camera_update(cam);
    }
}
