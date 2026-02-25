#include <stdlib.h>
#include <math.h>
#include "distributions.h"

// M_PI is not always available in the standard math library

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

/**
 * \file distributions.c
 * \author Omar DAHMAN
 * \date 2026-02-22
 * \brief Generate random numbers using various statistical distributions implementing distributions.h
 */


/**
 * \brief Function to get a random float beetween 0 and 1
 * \return float Random number in [0, 1]
 */
static float random_float(void) {
    return (float)rand() / (float)RAND_MAX;
}

float uniform(float a, float b) {
    return a + (b - a) * random_float();
}


/**
 * \brief Generates a random number following a normal (Gaussian) distribution.
 *
 * \details This function uses the Box-Muller method. 
 *
 * \return float A random number following the normal distribution N(mu, sigma).
 *
 * \note We use a loop to ensure we never calculate the log of 0
 * cosf(2.0f * M_PI * u2) th have a random angle 
 * sqrtf(-2.0f * logf(u1)) to have a random radius 
 */

float normal(float mu, float sigma) {
    float u1, u2, z;
    
    do {
        u1 = random_float();
    } while (u1 == 0.0f); 
    
    u2 = random_float();

    z = sqrtf(-2.0f * logf(u1)) * cosf(2.0f * M_PI * u2); 
    
    return mu + sigma * z;
}



/**
 * \brief Generates a random number following an exponential distribution.
 *
 * \details This function uses the inverse transform sampling method
 *
 * \note We use a loop to ensure we never calculate the log of 0
 */

float exponential(float k) {
    float u;
    
    do {
        u = random_float();
    } while (u == 0.0f);
    
    return -logf(u) / k;
}



/**
 * \brief Generates a random number from a discrete distribution.
 *
 * \details This function uses the cumulative probability method.
 *
 * \return float, the randomly selected value from array 't'.
 *
 * \note This last return :
 * Sometimes float numbers are not perfectly exact, so the sum of probabilities might not reach 1.0
 */

float discrete(int n, float *t, float *p) {
    float u, cumulative;
    int i;
    
    u = random_float();
    
    cumulative = 0.0f;
    for (i = 0; i < n; i++) {
        cumulative += p[i];
        if (u <= cumulative) {
            return t[i];
        }
    }

    return t[n - 1];
}
