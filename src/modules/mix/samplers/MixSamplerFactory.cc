#include <config.h>
#include "MixSamplerFactory.h"
#include "MixSampler.h"

MixSamplerFactory::MixSamplerFactory()
{}

static bool 
hasMarkedChild(Node *node, Graph const &graph, GraphMarks const &marks)
{
    set<Node*>::const_iterator i = node->children().begin();
    for (; i != node->children().end(); ++i) {
	if (graph.contains(*i) && marks.mark(*i) != 0) {
	    return true;
	}
    }
    return false;
}

void MixSamplerFactory::makeSampler(set<StochasticNode*> &nodes, 
				    Graph const &graph,
				    vector<vector<Sampler*> > &samplers) const
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
    vector<StochasticNode*> mix_nodes;
    for (p = graph.nodes().begin(); p != graph.nodes().end(); ++p) {
	if (marks.mark(*p) == 1) {
	    StochasticNode const *snode = asStochastic(*p);
	    if (snode && snode->distribution()->name() == "DNormMix") {
		mix_nodes.push_back(snode);
	    }
	}
    }
    if (mix_nodes.empty()) {
	return; //Nothing to do
    }

    /* Now find the stochastic parents of mix_nodes */
    for (unsigned int i = 0; i < mix_nodes.size(); ++i) {
	marks.markParents(mix_nodes[i], isStochastic, 2);
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
    for (p = graph.nodes().begin(); p != graph.nodes().end(); ++p) {
	if (marks.Mark(*p) == 2) {
	    bool cansample = true;
	    StochasticNode *snode = asStochastic(*p);
	    if (nodes.count(snode) == 0) {
		cansample = false;
	    }
	    else if (snode->isDiscreteValued()) {
		cansample = false;
	    }
	    else if (df(snode) != length(snode)) {
		/* FIXME: Excluding Dirichlet priors with this */
		cansample = false;
	    }
	    else {
		unsigned int Nparents = snode->parents().size();
		vector<bool> fixmask(Nparents);
		for (unsigned int i = 0; i < Nparents; i++) {
		    fixmask[i] = snode->parents()[i]->isObserved();
		}
		if (snode->distribution()->isSupportFixed(fixmask)) {
		    sample_nodes.push_back(snode);
		}
	    }
	}
    }

    if (sample_nodes.empty()) {
	return; //Nothing to do
    }
    else if (DSumSampler::canSample(sample_nodes, graph)) {
	for (unsigned int i = 0; i < sample_nodes.size(); ++i) {
	    nodes.erase(sample_nodes[i]);
	}
	for (unsigned int ch = 0; ch < samplers.size(); ++ch) {
	    samplers[ch].push_back(new MixSampler(sample_nodes, graph, ch));
	}
    }
}
