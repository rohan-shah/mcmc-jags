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
	bool checkOutcome(StochasticNode const *snode) const;
	bool checkLink(InverseLinkFunc const *link) const;
	GLMMethod *newMethod(Updater const *updater, unsigned int chain) 
	    const;
    };

}

#endif /* PROBIT_FACTORY_H_ */
