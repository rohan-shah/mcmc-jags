#ifndef MIX_SAMPLER_FACTORY_H_
#define MIX_SAMPLER_FACTORY_H_

#include <sampler/SamplerFactory.h>

namespace mix {

/**
 * @short Factory object for mixture samplers
 */
    class MixSamplerFactory : public SamplerFactory
    {
	std::string _name;
    public:
	MixSamplerFactory();
	Sampler * makeSampler(std::set<StochasticNode*> const &nodes, 
			      Graph const &graph) const;
	std::vector<Sampler*>  
	    makeSamplers(std::set<StochasticNode*> const &nodes, 
			 Graph const &graph) const;
	std::string const &name() const;
    };

}

#endif /* MIX_SAMPLER_FACTORY_H_ */
