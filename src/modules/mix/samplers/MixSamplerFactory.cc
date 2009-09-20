#include <config.h>
#include "MixSamplerFactory.h"
#include "MixSampler.h"
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/DensitySampler.h>

#include <set>

using std::set;
using std::vector;

static bool isStoch(Node const *node)
{
    return asStochastic(node);
}

//FIXME: This could be more generally useful - in library?
static 
vector<StochasticNode*> stochasticParents(Node const *node, 
					  StochasticNodeSet const &nodes,
					  Graph const &graph)
{
    GraphMarks marks(graph);
    marks.markParents(node, isStoch, 2);
    
    vector<StochasticNode*> parents;
    for (StochasticNodeSet::const_iterator q = nodes.begin();
	 q != nodes.end(); ++q)
    {
	if (marks.mark(*q) == 2) {
	    parents.push_back(*q);
	}
    }
    return parents;
}

namespace mix {

    Sampler * MixSamplerFactory::makeSampler(StochasticNodeSet const &nodes, 
					     Graph const &graph) const
    {
	vector<StochasticNode*> sparents;
        StochasticNodeSet::const_iterator p;
	for (p = nodes.begin(); p != nodes.end(); ++p) {
	    //Search candidate nodes for one with a stochastic child with distribution dnormmix
	    StochasticNodeSet const *stoch_children = (*p)->stochasticChildren();
	    StochasticNodeSet::const_iterator q;
	    for (q = stoch_children->begin(); q != stoch_children->end(); ++q) {
		if ((*q)->distribution()->name() == "dnormmix" && graph.contains(*q)) {
		    //Find all stochastic parents of the dnormmix node
		    sparents = stochasticParents(*q, nodes, graph);
		    //Can we sample them?
		    if (MixSampler::canSample(sparents, graph))
			break;
		}
	    }
	    if (q != stoch_children->end())
		break;
	}
	if (p == nodes.end()) {
	    return 0;
	}

	unsigned int nchain = sparents[0]->nchain();
	vector<DensityMethod*> methods(nchain,0);	    
	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    methods[ch] = new MixSampler(sparents);
	}
	return new DensitySampler(sparents, graph, methods);
    }

}
