#include <config.h>

#include "RealSlicer.h"
#include "DiscreteSlicer.h"
#include "SliceFactory.h"

#include <sampler/DensitySampler.h>
#include <sampler/Updater.h>
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

    Sampler *SliceFactory::makeSampler(StochasticNode *snode,
				       Graph const &graph) const
    {
	unsigned int nchain = snode->nchain();
	vector<DensityMethod*> methods(nchain, 0);
	Updater *updater = new Updater(snode, graph);

	bool discrete = snode->isDiscreteValued();
	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    if (discrete) {
		methods[ch] = new DiscreteSlicer(updater, ch);
	    }
	    else {
		methods[ch] = new RealSlicer(updater, ch);
	    }
	}

	return new DensitySampler(updater, methods);
    }

}
