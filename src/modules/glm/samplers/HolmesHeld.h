#ifndef HOLMES_HELD_H_
#define HOLMES_HELD_H_

#include "GLMBlock.h"

namespace jags {
namespace glm {

    /**
     * @short Holmes-Held sampler for binary GLMs
     *
     * Sampler for probit and logistic regression models with binary
     * outcome data, based on Holmes C and Held L (2006).  Bayesian
     * Auxiliary Variables Models for Binary and Multinomial
     * Regression, Bayesian Analysis, 1:148-168.
     */
    class HolmesHeld : public GLMBlock {
      public:
	/**
	 * Constructor.
	 * 
	 * @see GLMMethod#GLMMethod
	 */
	HolmesHeld(GraphView const *view, 
		   std::vector<SingletonGraphView const *> const &sub_views,
		   std::vector<Outcome *> const &outcomes,
		   unsigned int chain);
	/**
	 * Updates the auxiliary variables marginalizing over the
	 * value of the linear predictor. This function is called by
	 * GLMBlock#update.
	 *
	 * In the parent GLMBlock class, the posterior precision of
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
	 * and variance of the latent outcome z[], which forms a
	 * multivariate truncated normal distribution. Each element of
	 * z[] is updated in turn by Gibbs sampling.
	 */
	void updateAuxiliary(cholmod_dense *b, cholmod_factor *N, RNG *rng);
    };
    
}}

#endif /* HOLMES_HELD_H_ */
