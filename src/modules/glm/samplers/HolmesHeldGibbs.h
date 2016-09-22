#ifndef HOLMES_HELD_GIBBS_H_
#define HOLMES_HELD_GIBBS_H_

#include "GLMMethod.h"

namespace jags {
namespace glm {

    /**
     * @short Holmes Held sampler for binary GLMs
     *
     * Sampler for probit and logistic regression models with binary
     * outcome data, based on Holmes C and Held L (2006).  Bayesian
     * Auxiliary Variables Models for Binary and Multinomial
     * Regression, Bayesian Analysis, 1:148-168.
     */
    class HolmesHeldGibbs : public GLMMethod {
      public:
	/**
	 * Constructor.
	 * 
	 * @see GLMMethod#GLMMethod
	 */
	HolmesHeldGibbs(GraphView const *view, 
			std::vector<SingletonGraphView const *> const &sv,
			std::vector<Outcome *> const &outcomes,
			unsigned int chain);
	/**
	 * Updates the regression parameters element-wise (i.e. with
	 * Gibbs sampling).
	 */
	void update(RNG *rng);
    };
    
}}

#endif /* HOLMES_HELD_GIBBS_H_ */
