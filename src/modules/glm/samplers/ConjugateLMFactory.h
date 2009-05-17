#ifndef CONJUGATE_LM_FACTORY_H_
#define CONJUGATE_LM_FACTORY_H_

#include "GLMFactory.h"

namespace glm {

/**
 * @short Factory object for conjugate linear model sampler
 */
    class ConjugateLMFactory : public GLMFactory
    {
    public:
	bool checkOutcome(StochasticNode const *snode) const;
	bool checkLink(InverseLinkFunc const *link) const;
	GLMMethod *newMethod() const;
    };

}

#endif /* CONJUGATE_LM_FACTORY_H_ */
