#include <config.h>
#include "FiniteSampler.h"
#include "FiniteFactory.h"

namespace basesamplers {

    bool 
    FiniteFactory::canSample(StochasticNode * snode, Graph const &graph) const
    {
	return FiniteSampler::canSample(snode, graph);
    }

    Sampler *
    FiniteFactory::makeSingletonSampler(StochasticNode *snode,
					Graph const &graph,
					unsigned int chain) const
    {
	return new FiniteSampler(snode, graph, chain);
    }
    
}
