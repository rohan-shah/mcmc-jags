#ifndef LINEAR_GIBBS_FACTORY_H_
#define LINEAR_GIBBS_FACTORY_H_

#include "GLMFactory.h"

namespace jags {
namespace glm {

    /**
     * @short Factory for linear model sampler using Gibbs sampling
     *
     * Similar to LinearFactory, but produces a sampling method that
     * uses Gibbs sampling instead of block updating
     *
     * Note that the ability to update a linear model element-wise by
     * Gibbs sampling is already provided by the ConjugateNormal sampler
     * in the module "bugs".
     */
    class LinearGibbsFactory : public GLMFactory
    {
    public:
	LinearGibbsFactory();
	/**
	 * Checks that the outcome variable is from a normal family
	 * with identity link (i.e. lnode is NULL)
	 */
	bool checkOutcome(StochasticNode const *snode) const;
	/**
	 * Returns a newly allocated object of class Linear
	 */
	GLMMethod *newMethod(GraphView const *view, 
			     std::vector<SingletonGraphView const *> const &sub_views,
			     unsigned int chain) const;
    };

}}

#endif /* LINEAR_GIBBS_FACTORY_H_ */
