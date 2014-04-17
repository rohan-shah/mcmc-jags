#ifndef LDA_FACTORY_H_
#define LDA_FACTORY_H_

#include <sampler/SamplerFactory.h>

namespace jags {
    namespace mix {

	/**
	 * @short Factory object for LDA samplers
	 */

	class LDAFactory : public SamplerFactory
	{
	  public:
	    Sampler *
		makeSampler(std::vector<StochasticNode*> const &topicPriors,
			    std::vector<StochasticNode*> const &wordPriors,
			    std::list<StochasticNode*> const &free_nodes,
			    Graph const &graph) const;
	    std::vector<Sampler*> 
		makeSamplers(std::list<StochasticNode*> const &nodes, 
			     Graph const &graph) const;
	    std::string name() const;
	};
    }
}

#endif /* LDA_FACTORY_H_ */
