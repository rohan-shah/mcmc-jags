#ifndef LOGIT_FACTORY_H_
#define LOGIT_FACTORY_H_

#include "GLMFactory.h"

namespace glm {

    /**
     * @shortFactory object for conjugate linear model sampler
     */
    class LogitFactory : public GLMFactory
    {
    public:
	LogitFactory();
	bool checkOutcome(StochasticNode const *snode,
			  LinkNode const *lnode) const;
	GLMMethod *newMethod(Updater const *updater, 
			     std::vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const;
    };

}

#endif /* LOGIT_FACTORY_H_ */
