#ifndef BINARY_FACTORY_H_
#define BINARY_FACTORY_H_

#include "GLMFactory.h"

namespace jags {
namespace glm {

    /**
     * @short Base class for Binary GLM factories
     */
    class BinaryFactory : public GLMFactory
    {
    public:
	BinaryFactory(std::string const &name);
	/**
	 * Checks that the outcome variable is a Bernoulli random
	 * variable or a binomial variable with size fixed to 1, and
	 * the link function is logit or probit.  Linear models with
	 * identity link are also permitted.
	 */
	bool checkOutcome(StochasticNode const *snode) const;
	/**
	 * Returns a newly allocated object of class GLMMethod, unless
	 * we have a pure linear model with normal outcome variables.
	 * In this case, an object of class Linear is returned.
	 *
	 * This function is called by GLMFactory#makeSampler
	 */
	GLMMethod *
	    newMethod(GraphView const *view,
		      std::vector<SingletonGraphView const *> const &sub_views,
		      unsigned int chain, bool gibbs) const;
	/**
	 * Returns a newly allocated GLMMethod object.
	 */
	virtual GLMMethod *
	    newBinary(GraphView const *view,
		      std::vector<SingletonGraphView const *> const &sub_views,
		      std::vector<Outcome *> const &outcomes,
		      unsigned int chain, bool gibbs) const = 0;
    };

}}

#endif /* BINARY_FACTORY_H_ */
