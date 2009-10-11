#include <config.h>

#include "RealSlicer.h"
#include "DiscreteSlicer.h"
#include "SliceFactory.h"

#include <sampler/ParallelSampler.h>
#include <sampler/Updater.h>
#include <graph/StochasticNode.h>

#include <vector>

using std::vector;
using std::string;

namespace base {

    SliceFactory::SliceFactory()
	: _name("Slice")
    {}

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
	vector<SampleMethod*> methods(nchain, 0);
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

	return new ParallelSampler(updater, methods);
    }

    string const &SliceFactory::name() const
    {
	return _name;
    }
}
