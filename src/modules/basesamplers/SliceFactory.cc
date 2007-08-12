#include <config.h>

#include "RealSliceSampler.h"
#include "DiscreteSliceSampler.h"
#include "SliceFactory.h"

#include <sampler/ParallelDensitySampler.h>
#include <graph/StochasticNode.h>

#include <vector>

using std::vector;

namespace basesamplers {

    bool 
    SliceFactory::canSample(StochasticNode * snode, Graph const &graph) const
    {
	return snode->length() == 1 && df(snode) != 0;
    }

    Sampler *SliceFactory::makeSingletonSampler(StochasticNode *snode,
						Graph const &graph) const
    {
	unsigned int nchain = snode->nchain();
	vector<DensityMethod*> methods(nchain, 0);
	bool discrete = snode->isDiscreteValued();

	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    if (discrete) {
		methods[ch] = new DiscreteSlicer();
	    }
	    else {
		methods[ch] = new RealSlicer();
	    }
	}

	vector<StochasticNode*> nodes(1, snode);
	return new ParallelDensitySampler(nodes, graph, methods);
    }
}
