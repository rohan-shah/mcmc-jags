#include <config.h>

#include "RealSlicer.h"
#include "DiscreteSlicer.h"
#include "SliceFactory.h"

#include <sampler/MutableSampler.h>
#include <sampler/SingletonGraphView.h>
#include <graph/StochasticNode.h>

#include <vector>

using std::vector;
using std::string;

namespace jags {
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
	vector<MutableSampleMethod*> methods(nchain, 0);

	SingletonGraphView *gv = new SingletonGraphView(snode, graph);

	bool discrete = snode->isDiscreteValued();
	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    if (discrete) {
		methods[ch] = new DiscreteSlicer(gv, ch);
	    }
	    else {
		methods[ch] = new RealSlicer(gv, ch);
	    }
	}

	string name = discrete ? "base::DiscreteSlicer" : "base::RealSlicer";
	return new MutableSampler(gv, methods, name);
    }

    string SliceFactory::name() const
    {
	return "base::Slice";
    }

}}
