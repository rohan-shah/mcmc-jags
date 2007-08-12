#ifndef MIX_SAMPLER_FACTORY_H_
#define MIX_SAMPLER_FACTORY_H_

#include <sampler/SamplerFactory.h>

/**
 * @short Factory object for mixture samplers
 */
class MixSamplerFactory : public SamplerFactory
{
public:
    void makeSampler(std::set<StochasticNode*> &nodes, 
		     Graph const &graph,
		     std::vector<Sampler*> &samplers) const;
};

#endif /* MIX_SAMPLER_FACTORY_H_ */
