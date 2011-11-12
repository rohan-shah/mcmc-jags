#ifndef BINARY_FACTORY_H_
#define BINARY_FACTORY_H_

#include "GLMFactory.h"

namespace glm {

    class BinaryGLM;

    /**
     * @short Base class for Binary GLM factories
     */
    class BinaryFactory : public GLMFactory
    {
	bool _gibbs;
    public:
	BinaryFactory(std::string const &name, bool gibbs);
	/**
	 * Checks that the outcome variable is a Bernoulli random
	 * variable or a binomial variable with size fixed to 1, and
	 * the link function is logit or probit.  Linear models with
	 * identity link are also permitted.
	 */
	bool checkOutcome(StochasticNode const *snode,
			  LinkNode const *lnode) const;
	/**
	 * Returns a newly allocated object of class BinaryGLM, unless
	 * we have a pure linear model with normal outcome variables.
	 * In this case, an object of class Linear is returned, as 
	 * the overhead of the BinaryGLM class is not required.
	 *
	 * This function is called by GLMFactory#makeSampler
	 */
	GLMMethod *newMethod(GraphView const *view,
			     std::vector<GraphView const *> const &sub_views,
			     unsigned int chain) const;
	/**
	 * If the factory was constructed with parameter "gibbs" set
	 * to true, then snode must be scalar. Otherwise, snode must
	 * be unbounded.
	 *
	 * @param snode Candidate node for sampling by BinaryGLM
	 */
	bool canSample(StochasticNode const *snode) const;
	/**
	 * Returns a newly allocated BinaryGLM object.
	 */
	virtual BinaryGLM *newBinary(GraphView const *view,
				     std::vector<GraphView const *> const 
				     &sub_views,
				     unsigned int chain) const = 0;
    };

}

#endif /* BINARY_FACTORY_H_ */
