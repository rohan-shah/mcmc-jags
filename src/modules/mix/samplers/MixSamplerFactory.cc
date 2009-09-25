#include <config.h>
#include "MixSamplerFactory.h"
#include "MixSampler.h"
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/ParallelSampler.h>
#include <sampler/Updater.h>

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
	for (p = nodes.begin(); p != nodes.end(); ++p) {
	    //Search candidate nodes for one with a stochastic child with distribution dnormmix
	    set<StochasticNode*> const *stoch_children = (*p)->stochasticChildren();
	    set<StochasticNode*>::const_iterator q;
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

	Updater *updater = new Updater(sparents, graph);
	unsigned int nchain = sparents[0]->nchain();
	vector<SampleMethod*> methods(nchain,0);	    
	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    methods[ch] = new MixSampler(updater, ch);
	}
	return new ParallelSampler(updater, methods);
    }

}
