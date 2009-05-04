#ifndef CONJUGATE_LM_FACTORY_H_
#define CONJUGATE_LM_FACTORY_H_

#include <sampler/SamplerFactory.h>

namespace glm {

/**
 * @short Factory object for conjugate linear model sampler
 */
    class ConjugateLMFactory : public SamplerFactory
    {
    public:
	Sampler * makeSampler(std::set<StochasticNode*> const &nodes, 
			      Graph const &graph) const;
    };

}

#endif /* CONJUGATE_LM_FACTORY_H_ */
