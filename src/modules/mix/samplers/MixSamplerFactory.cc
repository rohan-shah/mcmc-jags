#include <config.h>
#include "MixSamplerFactory.h"
#include "NormMix.h"
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
	set<Node*> const &gnodes = graph.nodes();
	vector<Node const*> normmix_nodes;
        for (set<Node*>::const_iterator p = gnodes.begin(); 
	     p != gnodes.end(); ++p) 
	{
	    StochasticNode const *snode = asStochastic(*p);
	    if (snode && snode->distribution()->name() == "dnormmix") {
		normmix_nodes.push_back(*p);
	    }
	}
	
	for (unsigned int i = 0; i < normmix_nodes.size(); ++i) {

	    vector<StochasticNode*> sparents = 
		stochasticParents(normmix_nodes[i], nodes, graph);

	    if (NormMix::canSample(sparents, graph)) {
		
		Updater *updater = new Updater(sparents, graph);
		unsigned int nchain = sparents[0]->nchain();
		vector<SampleMethod*> methods(nchain,0);	    
		for (unsigned int ch = 0; ch < nchain; ++ch) {
		    methods[ch] = new NormMix(updater, ch);
		}
		return new ParallelSampler(updater, methods);		
	    }
	}

	return 0;
    }
}
