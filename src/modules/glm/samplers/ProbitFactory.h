#ifndef PROBIT_FACTORY_H_
#define PROBIT_FACTORY_H_

#include "GLMFactory.h"

namespace glm {

    /**
     * @shortFactory object for conjugate linear model sampler
     */
    class ProbitFactory : public GLMFactory
    {
    public:
	ProbitFactory();
	bool checkOutcome(StochasticNode const *snode,
			  LinkNode const *lnode) const;
	GLMMethod *newMethod(Updater const *updater, 
			     std::vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const;
    };

}

#endif /* PROBIT_FACTORY_H_ */
