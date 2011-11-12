#ifndef AM_FACTORY_H_
#define AM_FACTORY_H_

#include "GLMFactory.h"
#include "AMMethod.h"

namespace glm {

    /**
     * @short Factory object for the Auxiliary Mixture update method
     *
     * @see AMMethod
     */
    class AMFactory : public GLMFactory
    {
      public:
	AMFactory();
	/**
	 * Returns true if the outcome has a Poisson distribution with
	 * log link, or a Binomial distribution with logit link.
	 */
	bool checkOutcome(StochasticNode const *snode,
			  LinkNode const *lnode) const;
	/**
	 * Returns true if the node is not bounded
	 */
	bool canSample(StochasticNode const *snode) const;
	/**
	 * Returns a newly allocated object of class AMMethod for
	 * sampling Poisson models with log link or binomial models
	 * with logistic link.
	 */
	GLMMethod * newMethod(GraphView const *view,
			      std::vector<GraphView const *> const &sub_views, 
			      unsigned int chain) const;
    };

}

#endif /* AM_FACTORY_H_ */
