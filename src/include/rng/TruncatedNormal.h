#ifndef TRUNCATED_NORMAL_H_
#define TRUNCATED_NORMAL_H_

class RNG;

/**
 * Draws a random sample from a left-truncated standard normal
 * distribution.
 *
 * @param left Left limit of the truncated distribution
 * @param rng Pointer to a Random Number Generator
 */
double lnormal(double left, RNG *rng);

/**
 * Draws a random sample from a right-truncated standard normal
 * distribution.
 *
 * @param right Right limit of the distribution
 * @param rng Pointer to a Random Number Generator
 */
double rnormal(double right, RNG *rng);

/**
 * Draws a random sample from an interval-truncated standard
 * normal distribution.
 *
 * @param left Left limit of the distribution
 * @param right Right limit of the distribution
 * @param rng Pointer to a Random Number Generator
 */
double inormal(double left, double right, RNG *rng);

#endif /* TRUNCATED_NORMAL_H_ */
