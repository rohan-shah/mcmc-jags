#ifndef MIX_SAMPLER_FACTORY_H_
#define MIX_SAMPLER_FACTORY_H_

#include <sampler/SamplerFactory.h>

namespace mix {

/**
 * @short Factory object for mixture samplers
 */
    class MixSamplerFactory : public SamplerFactory
    {
    public:
	Sampler * makeSampler(std::set<StochasticNode*, less_snode> const &nodes, 
			      Graph const &graph) const;
    };

}

#endif /* MIX_SAMPLER_FACTORY_H_ */
