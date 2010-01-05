#ifndef LINEAR_GIBBS_FACTORY_H_
#define LINEAR_GIBBS_FACTORY_H_

#include "GLMFactory.h"

namespace glm {

/**
 * @short Factory object for conjugate linear model sampler
 */
    class LinearGibbsFactory : public GLMFactory
    {
	bool _gibbs;
    public:
	LinearGibbsFactory();
	bool checkOutcome(StochasticNode const *snode,
			  LinkNode const *lnode) const;
	GLMMethod *newMethod(GraphView const *view, 
			     std::vector<GraphView const *> const &sub_views,
			     unsigned int chain) const;
	bool canSample(StochasticNode const *snode) const;
    };

}

#endif /* LINEAR_GIBBS_FACTORY_H_ */
