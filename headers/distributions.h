/**
 * \file distributions.h
 * \brief Random number generation 
 * 
 * \details This file has the function declarations we need to generate
 * random numbers using various statistical distributions.
 */

#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

/**
 * \brief Generates a random number uniformly distributed
 * 
 * \details Just picks a random float beetween a and b, 
 * with equal chance for any value in that range.
 * 
 * \param a Lower bound 
 * \param b Upper bound
 * \return float A random number between a and b
 */
float uniform(float a, float b);

/**
 * \brief Generates a random number with normal (Gaussian) distribution
 * 
 * \details Most values will be close to mu,
 * and sigma controls how spread out they are.
 * 
 * \param mu The mean 
 * \param sigma Standard deviation 
 * \return float A random number following N(mu, sigma)
 */
float normal(float mu, float sigma);

/**
 * \brief Generates a random number with exponential distribution
 * 
 * \details Useful for modeling time between events that happen randomly
 * but at a constant average rate.
 * 
 * \param k The rate paramter 
 * \return float A random number from exponential distribution
 */
float exponential(float k);

/**
 * \brief Picks a random value from a discrete set of options
 * 
 * \details This function looks at a list of values and their chances. It then picks one value for you at random.
 * 
 * \attention The sum of all probabilities should add up to 1.
 * 
 * \param n How many elements are in the arrays (must be >= 1)
 * \param t Array of possible values to choose from
 * \param p Array of probabilities (should sum to 1, all postive)
 * \return float One of the values from array t
 */
float discrete(int n, float *t, float *p);

#endif 
