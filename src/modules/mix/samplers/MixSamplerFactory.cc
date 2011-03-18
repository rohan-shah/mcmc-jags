#include <config.h>
#include "MixSamplerFactory.h"
#include "NormMix.h"
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/ParallelSampler.h>
#include <sampler/GraphView.h>

#include <set>

using std::set;
using std::vector;
using std::string;

#define NLEVEL 200
#define MAX_TEMP 100
#define NREP 5

/* 
 * Returns a pointer to a newly allocated GraphView if snode has a
 * stochastic child with distribution "dnormmix", otherwise a null
 * pointer.
 */
static GraphView * isCandidate(StochasticNode *snode, Graph const &graph)
{
    GraphView *gv = new GraphView(snode, graph);
    vector<StochasticNode const*> const &schildren = gv->stochasticChildren();
    for (unsigned int i = 0; i < schildren.size(); ++i) {
	if (schildren[i]->distribution()->name() == "dnormmix") {
	    return gv;
	}
    }
    delete gv;
    return 0;
}

/*
 * Used to aggregate nodes with common stochastic children.
 */
static void aggregate(GraphView const *gv, vector<StochasticNode *> &nodes,
		      set<StochasticNode const*> &common_children)
{
    bool agg = nodes.empty();
    vector<StochasticNode const *> const &schildren = gv->stochasticChildren();
    for (unsigned int i = 0; i < schildren.size(); ++i) {
	if (common_children.count(schildren[i])) {
	    agg = true;
	    break;
	}
    }
    if (agg) {
	for (unsigned int i = 0; i < schildren.size(); ++i) {
	    common_children.insert(schildren[i]);
	}
	nodes.push_back(gv->nodes()[0]);
    }
    
}

namespace mix {

    Sampler * MixSamplerFactory::makeSampler(set<StochasticNode*> const &nodes, 
					     Graph const &graph) const
    {
	vector<GraphView*> gvec;
	for (set<StochasticNode*>::const_iterator p = nodes.begin();
	     p != nodes.end(); ++p)
	{
	    GraphView *gv = isCandidate(*p, graph);
	    if (gv) {
		gvec.push_back(gv);
	    }
	}
	if (gvec.empty())
	    return 0;

	vector<StochasticNode *> sample_nodes;
	set<StochasticNode const *> common_children;

	for (unsigned int i = 0; i < gvec.size(); ++i) {
	    aggregate(gvec[i], sample_nodes, common_children);
	    delete gvec[i];
	}
	if (NormMix::canSample(sample_nodes)) {
	    GraphView *gv = new GraphView(sample_nodes, graph, true);
	    unsigned int nchain = sample_nodes[0]->nchain();
	    vector<SampleMethod*> methods(nchain,0);	    
	    for (unsigned int ch = 0; ch < nchain; ++ch) {
		methods[ch] = new NormMix(gv, ch, NLEVEL, MAX_TEMP, NREP);
	    }
	    return new ParallelSampler(gv, methods);		
	}
	else {
	    return 0;
	}
    }

    string MixSamplerFactory::name() const
    {
	return "mix::TemperedMix";
    }

    vector<Sampler*>  
    MixSamplerFactory::makeSamplers(set<StochasticNode*> const &nodes, 
				    Graph const &graph) const
    {
	Sampler *s = makeSampler(nodes, graph);
	if (s) 
	    return vector<Sampler*>(1, s);
	else 
	    return vector<Sampler*>();
    }
}
