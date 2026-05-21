#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "environnement.h"
#include "distributions.h"
#include "optimization.h"
#include "particules.h"

#define WIDTH 300
#define HEIGHT 200
#define RADIUS 2.5
#define TIME_STEP 1.0
#define DURATION 20
#define INSTANCE_NUMBER 200

float find_min_distance(const env e) {
    int n = get_n(e);
    float min_distance;
    if (n<=1) {
        return 0.0f;
    }
    min_distance = distanceve(get_particule(e, 0), get_particule(e, 1));
    for (int i = 1; i < n; i++) {
        particule p = get_particule(e, i);
        for (int j = i+1; j < n; j++) {
            float distance = distanceve(p, get_particule(e, j));
            if (distance < min_distance) {
                min_distance = distance;
            }
        }
    }
    return min_distance;
}

FILE* setup_file(const char* name) {
    FILE* f = fopen(name, "w");
    if (f != NULL) {
        fprintf(f, "particule_number,ticks,distance_before,distance_after\n");
    }
    return f;
}

long time_taken_to_optimize(env environnement) {
    clock_t before = 0;
    clock_t after = 0;
    before = clock();
    init_env(environnement, DURATION);
    after = clock();
    return after - before;
}

void evaluate_type(const char* filename, int modulus, int increment) {
    FILE* f = setup_file(filename);
    if (f == NULL) {
        fprintf(stderr, "Error: cannot open %s\n", filename);
        return;
    }
    for (int i = modulus; i <= INSTANCE_NUMBER; i+=increment) {
        long ticks = 0;
        float unoptimized, optimized;
        // environment creation is supposed to be deterministic
        env e_base = create_environnement(i, WIDTH, HEIGHT, RADIUS, TIME_STEP);
        env e_optimized = create_environnement(i, WIDTH, HEIGHT, RADIUS, TIME_STEP);

        ticks = time_taken_to_optimize(e_optimized);

        for (int i = 0; i < DURATION; i++) {
            move_particules(e_base);
            move_particules(e_optimized);
        }

        unoptimized = find_min_distance(e_base);
        optimized = find_min_distance(e_optimized);

        fprintf(f, "%d,%ld,%f,%f\n", i, ticks, unoptimized, optimized);

        free_environnement(e_base);
        free_environnement(e_optimized);
    }
    fclose(f);
    fprintf(stdout, "Results correctly written to %s\n", filename);
}

int main(void) {
    srand((unsigned int) time(NULL));

    evaluate_type("results/result_symetrical.csv", 0, 4);
    evaluate_type("results/result_partial_symetrical.csv", 2, 4);
    evaluate_type("results/result_antisymetrical.csv", 3, 4);

    return EXIT_SUCCESS;
}