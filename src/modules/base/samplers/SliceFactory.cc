#include <config.h>

#include "RealSlicer.h"
#include "DiscreteSlicer.h"
#include "SliceFactory.h"

#include <sampler/DensitySampler.h>
#include <graph/StochasticNode.h>

#include <vector>

using std::vector;

namespace base {

    bool 
    SliceFactory::canSample(StochasticNode * node, Graph const &graph) const
    {
        if (node->isDiscreteValued()) {
            return DiscreteSlicer::canSample(node);
        }
        else {
            return RealSlicer::canSample(node);
        }
    }

    Sampler *SliceFactory::makeSingletonSampler(StochasticNode *snode,
						Graph const &graph) const
    {
	unsigned int nchain = snode->nchain();
	vector<DensityMethod*> methods(nchain, 0);
	bool discrete = snode->isDiscreteValued();

	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    if (discrete) {
		methods[ch] = new DiscreteSlicer(snode, ch);
	    }
	    else {
		methods[ch] = new RealSlicer();
	    }
	}

	vector<StochasticNode*> nodes(1, snode);
	return new DensitySampler(nodes, graph, methods);
    }

}
