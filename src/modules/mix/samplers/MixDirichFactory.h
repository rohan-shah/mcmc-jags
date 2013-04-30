#ifndef MIX_DIRICHLET_FACTORY_H_
#define MIX_DIRICHLET_FACTORY_H_

#include <sampler/SamplerFactory.h>

namespace jags {
    namespace mix {

	/**
	 * @short Factory object for MixDirich samplers
	 */
	class MixDirichFactory : public SamplerFactory
	{
	  public:
	    std::vector<Sampler*>  
		makeSamplers(std::set<StochasticNode*> const &nodes, 
			     Graph const &graph) const;
	    std::string name() const;
	};
    }
}

#endif /* MIX_DIRICHLET_FACTORY_H_ */
