#ifndef HOLMES_HELD_FACTORY_H_
#define HOLMES_HELD_FACTORY_H_

#include "GLMFactory.h"

namespace jags {
namespace glm {

    /**
     * @short Factory object for the Holmes-Held sampling method
     */
    class HolmesHeldFactory : public GLMFactory
    {
      public:
	HolmesHeldFactory();
	/**
	 * Checks that the outcome variable is a Bernoulli random
	 * variable or a binomial variable with size fixed to 1, and
	 * the link function is logit or probit.  Linear models with
	 * identity link are also permitted.
	 */
	bool checkOutcome(StochasticNode const *snode) const;
	/**
	 * Returns a newly allocated object of class
	 * HolmesHeld/HolmesHeldGibbs, unless we have a pure linear
	 * model with normal outcome variables.  In this case, an
	 * object of class GLMBlock/GLMGibbs is returned.
	 *
	 * This function is called by GLMFactory#makeSampler
	 */
	GLMMethod *
	    newMethod(GraphView const *view,
		      std::vector<SingletonGraphView const *> const &sub_views,
		      unsigned int chain, bool gibbs) const;
    };

}}

#endif /* HOLMES_HELD_FACTORY_H_ */
