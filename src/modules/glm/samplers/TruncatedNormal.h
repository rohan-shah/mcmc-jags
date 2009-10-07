#ifndef TRUNCATED_NORMAL_H_
#define TRUNCATED_NORMAL_H_

class RNG;

namespace glm {
     
    /**
     * Draws a random sample from a left-truncated normal distribution
     *
     * @param mu Mean of non-truncated distribution
     * @param sigma Standard deviation of non-truncated distribution
     * @param left Left limit of the truncated distribution
     * @param rng Random Number Generator
     */
    double LNorm(double mu, double sigma, double left, RNG *rng);

    /**
     * Draws a random sample from a left-truncated normal distribution
     *
     * @param mu Mean of non-truncated distribution
     * @param sigma Standard deviation of non-truncated distribution
     * @param right Right limit of the distribution
     * @param rng Random Number Generator
     */
    double RNorm(double mu, double sigma, double right, RNG *rng);

    /**
     * Draws a random sample from an interval-truncated normal
     * distribution
     *
     * @param mu Mean of non-truncated distribution
     * @param sigma Standard deviation of non-truncated distribution
     * @param left Left limit of the distribution
     * @param right Right limit of the distribution
     * @param rng Random Number Generator
     */
    double INorm(double mu, double sigma, double left, double right, 
		   RNG *rng);

}

#endif /* TRUNCATED_NORMAL_H_ */
