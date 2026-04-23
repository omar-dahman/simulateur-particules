/**
 * \file    evaluation.c
 * \author  JALOUALI RAYEN
 * \date    2026-04-23
 *
 * \brief   Experimental protocol - Task D.2
 * \details Evaluates the optimize() algorithm on 100 random instances.
 *
 *          For each instance, three measurements are taken:
 *            1. The optimized criterion: maximum distance between two particles
 *               at the first iteration exceeding T seconds, after optimize().
 *            2. The computation time of optimize() (in milliseconds).
 *            3. The improvement ratio: crit_baseline / crit_optimized.
 *               A ratio > 1 means the optimization improved the criterion.
 *
 *          The black box BN takes an already-positioned environment,
 *          simulates T seconds via move_particules() and returns the criterion.
 *
 *          Results are written to results/results.csv.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "environnement.h"
#include "optimization.h"
#include "distributions.h"


#define NB_INSTANCES  100    
#define N_MIN         2      
#define N_MAX         50     
#define W_MIN         10.0f 
#define W_MAX         200.0f 
#define H_MIN         10.0f  
#define H_MAX         200.0f
#define DT_MIN        0.1f   
#define DT_MAX        1.0f  
#define R_MIN         1.0f  
#define R_MAX         20.0f  
#define T_MIN         0.0f  
#define T_MAX         10.0f  

/**
 * \brief   Computes the maximum distance between all pairs of particles
 *
 * \param   e   environment as \a env
 * \return  maximum distance between two particles as \a float
 */
static float critere(env e) {
    int n = get_n(e);
    float max_d = 0.0f;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            float d = distanceve(get_particule(e, i), get_particule(e, j));
            if (d > max_d) max_d = d;
        }
    }
    return max_d;
}

/**
 * \brief   Black box BN: simulates T seconds and returns the criterion
 * \details Runs move_particules() until the cumulated time exceeds T seconds,
 *          then measures the maximum distance between particles.
 *
 * \param   e   already-positioned environment as \a env
 * \param   T   target duration as \a float
 * \return  measured criterion (max distance) as \a float
 */
static float BN(env e, float T) {
    /* If T = 0, measure directly without any iteration */
    if (T <= 0.0f) {
        return critere(e);
    }
    float elapsed = 0.0f;
    float dt = get_dt(e);
    while (elapsed <= T) {
        move_particules(e);
        elapsed += dt;
    }
    return critere(e);
}

/**
 * \brief   Evaluates one instance: compares baseline vs optimized
 * \details Creates two environments with the same parameters.
 *          One goes through optimize(), the other does not (baseline).
 *          Both are then passed to BN to measure the criterion.
 *
 * \param   n            number of particles
 * \param   w            environment width
 * \param   h            environment height
 * \param   r            interaction radius
 * \param   dt           iteration duration
 * \param   T            simulation duration
 * \param   crit_opt     (output) criterion with optimization
 * \param   crit_base    (output) criterion without optimization
 * \param   elapsed_ms   (output) computation time of optimize() in ms
 */
static void evaluate_instance(int n, float w, float h, float r, float dt,
                               float T,
                               float *crit_opt, float *crit_base,
                               double *elapsed_ms) {
    /* Baseline: default behavior of create_environnement */
    env e_base = create_environnement(n, w, h, r, dt);
    *crit_base = BN(e_base, T);
    free_environnement(e_base);

    /* Optimized: call optimize() before BN */
    env e_opt = create_environnement(n, w, h, r, dt);

    /* Measure computation time of optimize() */
    clock_t t0 = clock();
    optimize(e_opt, T);
    clock_t t1 = clock();
    *elapsed_ms = (double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0;

    *crit_opt = BN(e_opt, T);
    free_environnement(e_opt);
}

int main(void) {
    srand((unsigned int)time(NULL));

    FILE *f = fopen("results/results.csv", "w");
    if (!f) {
        fprintf(stderr, "Error: cannot open results/results.csv\n");
        return EXIT_FAILURE;
    }

    /* CSV header */
    fprintf(f, "instance,n,w,h,r,dt,T,"
               "crit_baseline,crit_optimise,temps_ms,ratio_amelioration\n");

    printf("Running %d instances...\n", NB_INSTANCES);

    for (int inst = 0; inst < NB_INSTANCES; inst++) {
        /* Randomly generate parameters uniformly within defined ranges */
        int   n  = N_MIN + rand() % (N_MAX - N_MIN + 1);
        float w  = uniform(W_MIN, W_MAX);
        float h  = uniform(H_MIN, H_MAX);
        float r  = uniform(R_MIN, R_MAX);
        float dt = uniform(DT_MIN, DT_MAX);
        float T  = uniform(T_MIN, T_MAX);

        float  crit_opt, crit_base;
        double elapsed_ms;

        evaluate_instance(n, w, h, r, dt, T,
                          &crit_opt, &crit_base, &elapsed_ms);

        /* Improvement ratio: >1 means optimization improved the criterion */
        float ratio = (crit_opt > 1e-6f) ? (crit_base / crit_opt) : 0.0f;

        fprintf(f, "%d,%d,%.2f,%.2f,%.2f,%.4f,%.2f,%.6f,%.6f,%.4f,%.6f\n",
                inst + 1, n, w, h, r, dt, T,
                crit_base, crit_opt, elapsed_ms, ratio);

        if ((inst + 1) % 10 == 0)
            printf("  %d/%d instances processed\n", inst + 1, NB_INSTANCES);
    }

    fclose(f);
    printf("Results written to results/results.csv\n");
    return EXIT_SUCCESS;
}