#ifndef LINEAR_FACTORY_H_
#define LINEAR_FACTORY_H_

#include "GLMFactory.h"

namespace glm {

    /**
     * @short Factory object for conjugate linear model sampling method
     */
    class LinearFactory : public GLMFactory
    {
	bool _gibbs;
    public:
	LinearFactory();
	/**
	 * Checks that the outcome is from the normal family with identity
	 * link (i.e. lnode is NULL)
	 */
	bool checkOutcome(StochasticNode const *snode,
			  LinkNode const *lnode) const;
	/**
	 * Returns a newly allocated object of class Linear
	 */
	GLMMethod *newMethod(GraphView const *view, 
			     std::vector<GraphView const *> const &sub_views,
			     unsigned int chain) const;
	/**
	 * Checks that the candidate node is not bounded. This is
	 * required for block-updating from the joint multivariate
	 * normal posterior.
	 */
	bool canSample(StochasticNode const *snode) const;
    };

}

#endif /* LINEAR_FACTORY_H_ */
