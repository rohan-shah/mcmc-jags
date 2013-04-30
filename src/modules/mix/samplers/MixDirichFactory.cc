#include <config.h>
#include "MixDirichFactory.h"
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

//STL syntax becomes unreadable if we don't use a typedef
typedef map<vector<StochasticNode const *>, vector<StochasticNode*> > MDMap;

namespace jags {

    static bool isCandidate(GraphView const &gv)
    {
	vector<StochasticNode const*> const &schild = gv.stochasticChildren();
	vector<DeterministicNode *> const &dchild = gv.deterministicChildren();

	//A necessary but not sufficient condition: we do this first
	//because it is fast
	if (schild.size() != dchild.size()) return false;
	
	//Stochastic children must all have dcat distribution
	for (unsigned int i = 0; i < schild.size(); ++i) {
	    if (schild[i]->distribution()->name() != "ddirch") return false;
	}
	
	//Deterministic descendants must all be mixture nodes
	for (unsigned int j = 0; j < dchild.size(); ++j) {
	    if (!isMixture(dchild[j])) return false;
	}
	
	//Mixture nodes cannot have any deterministic children in the
	//graph
	for (unsigned int j = 0; j < dchild.size(); ++j) {
	    set<DeterministicNode*> const *dj = 
		dchild[j]->deterministicChildren();
	    //Note: in general dj will be empty, so this is an
	    //expensive but rarely used test
	    for (set<DeterministicNode*>::const_iterator p = dj->begin();
		 p != dj->end(); ++p) 
	    {
		//dchild is in topological order, so we only need to
		//check for nodes with a higher index
		for (unsigned int k = j+1, k < dchild.size(); ++k) {
		    if (*p == dchild[k]) return false;
		}
	    }
	}
	return true;
    }

    namespace mix {
	
	Sampler * 
	BlockDirichFactory::makeSampler(vector<StochasticNode*> const &snodes, 
					Graph const &graph) const
	{
	    GraphView *gv = new GraphView(sample_nodes, graph);

	    if (MixDirich::canSample(gv)) {
		vector<SampleMethod*> methods(nchain);	    
		for (unsigned int ch = 0; ch < nchain; ++ch) {
		    methods[ch] = new MixDirich(gv, ch);
		}
		return new ParallelSampler(gv, methods);		
	    }
	    else {
		delete gv;
		return 0;
	    }
	}

	string MixDirichFactory::name() const
	{
	    return "mix::MixDirich";
	}

	vector<Sampler*>  
	MixDirichFactory::makeSamplers(set<StochasticNode*> const &nodes, 
				      Graph const &graph) const
	{
	    //Assemble candidates from available nodes and classify
	    //them by their stochastic children
	    MDMap cmap;

	    for (set<StochasticNode*>::const_iterator p = nodes.begin();
		 p != nodes.end(); ++p)
	    {
		if ((*p)->distribution()->name() != "ddirch") continue;

		GraphView gv(*p, graph);
		if (isCandidate(gv)) {
		    MDMap::iterator q = cmap.find(gv.stochasticChildren());
		    if (q == cmap.end()) {
			//create a new entry
			MDMap::value_type r(gv.stochasticChildren(),
					    set<StochasticNode*>());
			q = cmap.insert(r)->first;
		    }
		    q->second.push_back(snode);
		}
	    }
	    
	    //Now traverse the candidate map and generate samplers
	    vector<Sampler*> samplers;
	    for (MDMap::const_iterator q = cmap.begin(); q != cmap.end(); ++q)
	    {
		Sampler *s = makeSampler(q->second, graph);		
		if (s) samplers.push_back(s);
	    }

	    return samplers;
	}

    }
}
