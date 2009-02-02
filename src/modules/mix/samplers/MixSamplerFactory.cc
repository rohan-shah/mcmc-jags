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
					  set<StochasticNode*> const &nodes,
					  Graph const &graph)
{
    GraphMarks marks(graph);
    marks.markParents(node, isStoch, 2);
    
    vector<StochasticNode*> parents;
    for (set<StochasticNode*>::const_iterator q = nodes.begin();
	 q != nodes.end(); ++q)
    {
	if (marks.mark(*q) == 2) {
	    parents.push_back(*q);
	}
    }
    return parents;
}

namespace mix {

    Sampler * MixSamplerFactory::makeSampler(set<StochasticNode*> const &nodes, 
					     Graph const &graph) const
    {
	vector<StochasticNode*> sparents;
        set<StochasticNode*>::const_iterator p;
	for (p = graph.stochasticNodes().begin(); p != graph.stochasticNodes().end(); ++p) {
	 
	    if ((*p)->distribution()->name() == "dnormmix") {
		sparents = stochasticParents(*p, nodes, graph);
		if (MixSampler::canSample(sparents, graph))
		    break;
	    }
	}
	if (p == graph.stochasticNodes().end()) {
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
