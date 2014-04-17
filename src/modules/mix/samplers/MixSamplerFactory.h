#ifndef MIX_SAMPLER_FACTORY_H_
#define MIX_SAMPLER_FACTORY_H_

#include <sampler/SamplerFactory.h>

namespace jags {
namespace mix {

/**
 * @short Factory object for mixture samplers
 */
    class MixSamplerFactory : public SamplerFactory
    {
    public:
	Sampler * makeSampler(std::list<StochasticNode*> const &nodes, 
			      Graph const &graph) const;
	std::vector<Sampler*>  
	    makeSamplers(std::list<StochasticNode*> const &nodes, 
			 Graph const &graph) const;
	std::string name() const;
    };

}}

#endif /* MIX_SAMPLER_FACTORY_H_ */
