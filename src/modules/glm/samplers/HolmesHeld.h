#ifndef HOLMES_HELD_H_
#define HOLMES_HELD_H_

#include "BinaryGLM.h"

namespace glm {

    /**
     * @short Holmes Held sampler for binary GLMs
     *
     * Sampler for probit and logistic regression models with binary
     * outcome data, based on Holmes C and Held L (2006).  Bayesian
     * Auxiliary Variables Models for Binary and Multinomial
     * Regression, Bayesian Analysis, 1:148-168.
     *
     * In the Holmes-Held sampling method auxiliary normal variables
     * (z[i]) are sampled from their joint distribution after
     * marginalizing over the regression parameters of the GLM.  In
     * probit models, this provides improved mixing over the
     * Albert-Chib algorithm (See class AlbertChib). In logistic
     * regression models, z[i] has a logistic distribution, which is
     * represented by a scale mixture of normals.  The scale parameter
     * (tau[i]) is sampled by the update method.  
     *
     * For logistic regression the improvement over the standard
     * Albert-Chib algorithm is not so clear cut.
     */
    class HolmesHeld : public BinaryGLM {
	bool _aux_init; //Do we need to initialize auxiliary variables?
    public:
	/**
	 * Constructor.
	 * 
	 * @see GLMMethod#GLMMethod
	 */
	HolmesHeld(GraphView const *view, 
		   std::vector<GraphView const *> const &sub_views,
		   unsigned int chain);
	/**
	 * Updates the auxiliary variables (z[]) provided by the
	 * parent BinaryGLM class. This function is called by
	 * GLMMethod#updateLM.
	 *
	 * In the parent GLMMethod class, the posterior precision of
	 * the regression parameters is represented by the matrix "A"
	 * and the posterior mean "mu" solves A %*% mu = b.
	 *
	 * In this call, "N" holds the factorization 
	 * P %*% A %*% t(P) = L %*% D %*% t(L)
         * where P is a permutation matrix chosen to make
	 * the factorization more efficient, and "w" solves
	 * the equation L %*% w = P %*% b,
	 *
	 * These values are then used to calculate the marginal mean
	 * and variance of z[], which forms a multivariate truncated
	 * normal distribution. Each element of z[] is updated in turn
	 * by Gibbs sampling.
	 */
	void updateAuxiliary(cholmod_dense *b, cholmod_factor *N, RNG *rng);
	/**
	 * Returns "Holmes-Held"
	 */
	std::string name() const;
	/**
	 * The update takes place in two steps. Firstly, for logistic
	 * regression, the auxiliary variables tau[i] (representing
	 * the precision of the latent variable z[i]) are updated
	 * using the current values of z[], then GLMMethod#updateLM is
	 * called. For probit regression, tau[i] is fixed at 1.
	 */
	void update(RNG *rng);
    };
    
}

#endif /* HOLMES_HELD_H_ */
