#include <config.h>
#include <sarray/nainf.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <sampler/ParallelDensitySampler.h>
#include <sampler/DensityMethod.h>

#include "FiniteMethod.h"
#include "FiniteFactory.h"

#include <vector>

using std::vector;

namespace base {

    bool 
    FiniteFactory::canSample(StochasticNode *snode, Graph const &graph) const
    {
	return FiniteMethod::canSample(snode);
    }

    Sampler *  FiniteFactory::makeSingletonSampler(StochasticNode *snode,
						   Graph const &graph) const
    {
	unsigned int nchain = snode->nchain();
	vector<DensityMethod*> methods(nchain, 0);
	for (unsigned int i = 0; i < nchain; ++i) {
	    methods[i] = new FiniteMethod(snode);
	}
	
	vector<StochasticNode*> sample_nodes(1, snode);
	return new ParallelDensitySampler(sample_nodes, graph, methods);
    }
    
}
