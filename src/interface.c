#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/interface.h"

/*
 * Checks if a pixel at position (px, py) in an image of size wp x hp
 * is occupied by at least one particle in the environment.
 * Converts pixel coordinates to environment coordinates and compares
 * them with each particle's position.
 * Returns 1 if occupied, 0 otherwise.
 */
static int is_pixel_occupied(environnement env, int px, int py, int wp, int hp)
{
    /* Convert pixel coordinates to environment coordinates */
    float env_x = (float)px / (float)wp * env.width;
    float env_y = (float)py / (float)hp * env.height;

    /* Size of one pixel in environment units */
    float pixel_w = env.width / (float)wp;
    float pixel_h = env.height / (float)hp;

    int i;
    for (i = 0; i < env.nb_particules; i++)
    {
        float part_x = env.particules[i].pos_x;
        float part_y = env.particules[i].pos_y;

        if (part_x >= env_x && part_x < env_x + pixel_w &&
            part_y >= env_y && part_y < env_y + pixel_h)
        {
            return 1;
        }
    }
    return 0;
}

/*
 * Writes the header of a PBM file in P1 (ASCII) format.
 * The header contains the magic number "P1" followed by
 * the image width and height.
 */
static void write_pbm_header(FILE *file, int wp, int hp)
{
    fprintf(file, "P1\n");
    fprintf(file, "%d %d\n", wp, hp);
}

/*
 * Writes the pixel matrix of a PBM file based on the current
 * state of the environment. Each pixel is 1 if a particle
 * occupies that position, 0 otherwise.
 * Pixels on the same row are separated by spaces.
 */
static void write_pbm_body(FILE *file, environnement env, int wp, int hp)
{
    int x, y;
    for (y = 0; y < hp; y++)
    {
        for (x = 0; x < wp; x++)
        {
            if (x > 0)
                fprintf(file, " ");
            fprintf(file, "%d", is_pixel_occupied(env, x, y, wp, hp));
        }
        fprintf(file, "\n");
    }
}

/*
 * Builds the filename for a specific animation frame.
 * The resulting format is: <path>/env-<iteration>.pbm
 * The result is written into dest, up to max_len characters.
 */
static void build_frame_filename(char *dest, const char *path, int iteration, int max_len)
{
    snprintf(dest, max_len, "%s/env-%d.pbm", path, iteration);
}

/*
 * Prints the information of a single particle to standard output,
 * including its position, speed, mass and radius.
 */
void print_particule(particule p)
{
    printf("Particule :\n");
    printf("  Position : (%f, %f)\n", p.pos_x, p.pos_y);
    printf("  Vitesse  : (%f, %f)\n", p.vit_x, p.vit_y);
    printf("  Masse    : %f\n", p.masse);
    printf("  Rayon    : %f\n", p.rayon);
}

/*
 * Prints all the information of an environment to standard output,
 * including its dimensions, number of particles, time step,
 * interaction radius, and the details of each particle.
 */
void print_environnement(environnement env)
{
    int i;

    printf("=== Environnement ===\n");
    printf("  Taille : %f x %f\n", env.width, env.height);
    printf("  Nombre de particules : %d\n", env.nb_particules);
    printf("  dt : %f\n", env.dt);
    printf("  r  : %f\n", env.r);
    printf("---------------------\n");

    for (i = 0; i < env.nb_particules; i++)
    {
        printf("Particule [%d] :\n", i);
        print_particule(env.particules[i]);
        printf("\n");
    }
    printf("=====================\n");
}

/*
 * Prints a message string to standard output.
 * This file is the only one allowed to explicitly use standard output.
 */
void print_message(char *message)
{
    printf("%s\n", message);
}

/*
 * Prompts the user and reads an integer from standard input.
 * This file is the only one allowed to explicitly use standard input.
 */
int get_int()
{
    int value;
    printf("Entrez un entier : ");
    scanf("%d", &value);
    return value;
}

/*
 * Prompts the user and reads a float from standard input.
 * This file is the only one allowed to explicitly use standard input.
 */
float get_float()
{
    float value;
    printf("Entrez un flottant : ");
    scanf("%f", &value);
    return value;
}

/*
 * Writes a PBM image file of size wp x hp representing the current
 * state of the environment. The file is created at the given path.
 * If the file cannot be opened, an error message is printed.
 */
void snapshot(environnement env, char *path, int wp, int hp)
{
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", path);
        return;
    }

    write_pbm_header(file, wp, hp);
    write_pbm_body(file, env, wp, hp);

    fclose(file);
}

/*
 * Generates t PBM image files representing the environment over
 * t iterations of movement. Each file is named env-i.pbm where i
 * is the iteration number, and is saved in the directory given by path.
 * After each snapshot, the particles are moved for the next iteration.
 */
void animate(environnement env, char *path, int wp, int hp, int t)
{
    char filename[512];
    int i;

    for (i = 0; i < t; i++)
    {
        /* Build the filename for this iteration */
        build_frame_filename(filename, path, i, 512);

        /* Write the snapshot of the current state */
        snapshot(env, filename, wp, hp);

        /* Move the particles for the next iteration */
        deplacer(env);
    }
}
