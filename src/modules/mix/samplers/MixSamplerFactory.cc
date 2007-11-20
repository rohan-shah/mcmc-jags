#include <config.h>
#include "MixSamplerFactory.h"
#include "MixSampler.h"
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/ParallelDensitySampler.h>

#include <set>

using std::set;
using std::vector;

static bool isStoch(Node const *node)
{
    return asStochastic(node);
}

namespace mix {

    void MixSamplerFactory::makeSampler(set<StochasticNode*> &nodes, 
					Graph const &graph,
					vector<Sampler *> &samplers) const
    {
	GraphMarks marks(graph);

	/* Find observed nodes in the graph and mark their ancestors:
	   these are the informative nodes */
	set<Node*>::const_iterator p;
	for (p = graph.nodes().begin(); p != graph.nodes().end(); ++p) {
	    if ((*p)->isObserved()) {
		marks.mark(*p, 1);
		marks.markAncestors(*p, 1);
	    }
	}

	/* Find informative nodes in the graph with distribution DNormMix
	   and add them to the vector "mix_nodes" */
	vector<StochasticNode const*> mix_nodes;
	for (p = graph.nodes().begin(); p != graph.nodes().end(); ++p) {
	    if (marks.mark(*p) == 1) {
		StochasticNode const *snode = asStochastic(*p);
		if (snode && snode->distribution()->name() == "dnormmix") {
		    mix_nodes.push_back(snode);
		}
	    }
	}
	if (mix_nodes.empty()) {
	    return; //Nothing to do
	}

	/* Now find the stochastic parents of mix_nodes */
	for (unsigned int i = 0; i < mix_nodes.size(); ++i) {
	    marks.markParents(mix_nodes[i], isStoch, 2);
	}
	/* The stochastic parents can be sampled if
	   1) They are in the given set of nodes
	   2) They are not discrete valued
	   3) They have no dimensional constraints 
	   4) They have fixed support.
    
	   Condition 4 is necessary because the sampler uses log or logit
	   transformation of bounded variables, which must remain constant
	   between iterations to ensure stationarity of the chain.
	*/
	vector<StochasticNode*> sample_nodes;
	set<StochasticNode*>::const_iterator q;
	for (q = nodes.begin(); q != nodes.end(); ++q) {
	    StochasticNode *snode = *q;
	    bool cansample = true;
	    if (marks.mark(snode) != 2) {
		cansample = false;
	    }
	    else if (snode->isDiscreteValued()) {
		cansample = false;
	    }
	    else if (df(snode) != (snode)->length()) {
		/* FIXME: Excluding Dirichlet priors with this */
		cansample = false;
	    }
	    else if(!isSupportFixed(snode)) {
		cansample = false;
	    }
	    if (cansample) {
		sample_nodes.push_back(snode);
	    }
	}

	if (sample_nodes.empty()) {
	    return; //Nothing to do
	}
	else if (MixSampler::canSample(sample_nodes, graph)) {

	    for (unsigned int i = 0; i < sample_nodes.size(); ++i) {
		nodes.erase(sample_nodes[i]);
	    }

	    unsigned int nchain = sample_nodes[0]->nchain();
	    vector<DensityMethod*> methods(nchain,0);	    
	    for (unsigned int ch = 0; ch < nchain; ++ch) {
		methods[ch] = new MixSampler(sample_nodes);
	    }
	    Sampler *mix = new ParallelDensitySampler(sample_nodes, graph, 
						      methods);
	    samplers.push_back(mix);
	}
    }

}
