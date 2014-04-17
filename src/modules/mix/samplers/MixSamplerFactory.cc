#include <config.h>
#include "MixSamplerFactory.h"
#include "NormMix.h"
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/MutableSampler.h>
#include <sampler/SingletonGraphView.h>

using std::set;
using std::list;
using std::vector;
using std::string;

#define NLEVEL 200
#define MAX_TEMP 100
#define NREP 5

namespace jags {

/* 
 * Returns a pointer to a newly allocated SingletonGraphView if snode
 * has a stochastic child with distribution "dnormmix", otherwise a
 * null pointer.
 */
static SingletonGraphView * isCandidate(StochasticNode *snode, 
					Graph const &graph)
{
    SingletonGraphView *gv = new SingletonGraphView(snode, graph);
    vector<StochasticNode *> const &schildren = gv->stochasticChildren();
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
static void aggregate(SingletonGraphView const *gv, 
		      vector<StochasticNode *> &nodes,
		      set<StochasticNode const*> &common_children)
{
    bool agg = nodes.empty();
    vector<StochasticNode *> const &schildren = gv->stochasticChildren();
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
	nodes.push_back(gv->node());
    }
    
}

namespace mix {

    Sampler * 
    MixSamplerFactory::makeSampler(list<StochasticNode*> const &nodes, 
				   Graph const &graph) const
    {
	vector<SingletonGraphView*> gvec;
	for (list<StochasticNode*>::const_iterator p = nodes.begin();
	     p != nodes.end(); ++p)
	{
	    SingletonGraphView *gv = isCandidate(*p, graph);
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
	    vector<MutableSampleMethod*> methods(nchain,0);	    
	    for (unsigned int ch = 0; ch < nchain; ++ch) {
		methods[ch] = new NormMix(gv, ch, NLEVEL, MAX_TEMP, NREP);
	    }
	    return new MutableSampler(gv, methods, "mix::NormMix");		
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
    MixSamplerFactory::makeSamplers(list<StochasticNode*> const &nodes, 
				    Graph const &graph) const
    {
	Sampler *s = makeSampler(nodes, graph);
	if (s) 
	    return vector<Sampler*>(1, s);
	else 
	    return vector<Sampler*>();
    }

}}
