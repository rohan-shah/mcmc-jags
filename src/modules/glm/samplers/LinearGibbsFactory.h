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
	GLMMethod *newMethod(Updater const *updater, 
			     std::vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const;
	bool trunc() const;
    };

}

#endif /* LINEAR_GIBBS_FACTORY_H_ */
