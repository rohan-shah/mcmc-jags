#ifndef PG_H_
#define PG_H_

namespace jags {

    struct RNG;
    
    namespace glm {
	/**
	 * Sample from a inverse Gaussian distribution truncated to (0,t)
	 *
	 * @param mu Mean of the inverse Gaussian distribution
	 * @param lambda Shape parameter for the inverse Gaussian distribution
	 * @param t Truncation point
	 * @param rng Random number generator
	 */
	double rigauss(double mu, double lambda, double t, RNG *rng);
    }
}

#endif /* PG_H_ */
