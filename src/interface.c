#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "interface.h"
#include "environnement.h"
#include "particules.h"

void print_particule(particule p)
{
    if (p == NULL)
        return;

    printf("Particule :\n");
    printf("  Position : (%f , %f)\n", get_x(p), get_y(p));
    printf("  Vitesse  : (%f , %f)\n", get_vx(p), get_vy(p));
}

void print_environnement(env e)
{
    if (e == NULL)
        return;

    printf("Environnement :\n");
    printf("  Nombre de particules : %d\n", get_n(e));
    printf("  Largeur  : %f\n", get_w(e));
    printf("  Hauteur  : %f\n", get_h(e));
    printf("  dt       : %f\n", get_dt(e));
    printf("  Rayon R  : %f\n\n", get_r(e));

    for (int i = 0; i < get_n(e); i++)
    {
        printf("Particule %d :\n", i);
        print_particule(get_particule(e, i));
        printf("\n");
    }
}

void print_message(char *message)
{
    if (message != NULL)
        printf("%s", message);
}


int get_int()
{
    int value;
    int ret;

    do
    {
        ret = scanf("%d", &value);

        if (ret != 1)
        {
            printf("Entrée invalide. Veuillez entrer un entier : ");
            while (getchar() != '\n'); // nettoyage buffer
        }

    } while (ret != 1);

    return value;
}

float get_float()
{
    float value;
    int ret;

    do
    {
        ret = scanf("%f", &value);

        if (ret != 1)
        {
            printf("Entrée invalide. Veuillez entrer un float : ");
            while (getchar() != '\n'); // nettoyage buffer
        }

    } while (ret != 1);

    return value;
}


static int is_pixel_occupied(env e, int px, int py, int wp, int hp)
{
    int n = get_n(e);

    for (int i = 0; i < n; i++)
    {
        particule p = get_particule(e, i);

        float x = get_x(p);
        float y = get_y(p);

        int pixel_x = (int)(x * wp / get_w(e));
        int pixel_y = (int)(y * hp / get_h(e));

        if (pixel_x == px && pixel_y == py)
            return 1;
    }

    return 0;
}

void snapshot(env e, char *path, int wp, int hp)
{
    FILE *file = fopen(path, "w");
    if (file == NULL)
        return;

    fprintf(file, "P1\n");
    fprintf(file, "# Snapshot environnement\n");
    fprintf(file, "%d %d\n", wp, hp);

    for (int y = 0; y < hp; y++)
    {
        for (int x = 0; x < wp; x++)
        {
            fprintf(file, "%d ", is_pixel_occupied(e, x, y, wp, hp));
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void animate(env e, char *path, int wp, int hp, int t)
{
    char filename[256];

    for (int i = 0; i < t; i++)
    {
        snprintf(filename, sizeof(filename), "%s/env-%d.pbm", path, i);
        snapshot(e, filename, wp, hp);
        move_particules(e);
    }
}
