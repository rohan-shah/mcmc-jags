#ifndef LINEAR_FACTORY_H_
#define LINEAR_FACTORY_H_

#include "GLMFactory.h"

namespace glm {

/**
 * @short Factory object for conjugate linear model sampler
 */
    class LinearFactory : public GLMFactory
    {
	bool _gibbs;
    public:
	LinearFactory();
	bool checkOutcome(StochasticNode const *snode,
			  LinkNode const *lnode) const;
	GLMMethod *newMethod(GraphView const *view, 
			     std::vector<GraphView const *> const &sub_views,
			     unsigned int chain) const;
	bool canSample(StochasticNode const *snode) const;
    };

}

#endif /* LINEAR_FACTORY_H_ */
