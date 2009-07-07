#include <config.h>

#include "CutFactory.h"
#include "CutSampler.h"

#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <graph/LinkNode.h>
#include <distribution/Distribution.h>
#include <sampler/DensitySampler.h>
#include <sampler/Linear.h>

#include <set>
#include <map>
#include <algorithm>

using std::set;
using std::vector;
using std::map;
using std::stable_sort;
using std::string;


static void checkChildren(StochasticNode *snode, Graph const &graph, 
			  set<StochasticNode*> &sample_nodes)
{
    //Check to see if cutnode has any unobserved children that have
    //already been assigned a sampler, and are hence absent from the
    //set of sample nodes.

    set<StochasticNode const*> children = getStochasticChildren(snode, graph);
    
    for (set<StochasticNode*>::const_iterator p = children.begin();
	 p != children.end(); ++p)
    {
	if (!(*p)->isObserved() && sample_nodes.count(*p) == 0) {
	    throw runtime_error("The cut module must be loaded last.");
	}
    }
}

/*
  Aggregates cut nodes with common unobserved children into a joint model. 
  
  Modifies the arguments in place and returns true the status of
  the candidate node can be resolved.
*/
static bool aggregateCut(StochasticNode *candidate_node,
			 vector<StochasticNode*> &sample_nodes,
			 set<StochasticNode const*> &stochastic_children,
			 Graph const &graph)
{

    // Check that there is some overlap in stochastic children between
    // candidate node and current set.  If no, then we have to defer
    // judgement.

    set<StochasticNode const*> stoch_nodes;
    Sampler::getStochasticChildren(vector<StochasticNode*>(1, candidate_node),
				   graph, stoch_nodes);

    bool overlap = false;
    for (unsigned int i = 0; i < stoch_nodes.size(); ++i) {
	if (stochastic_children.count(stoch_nodes[i]) > 0) {
	    overlap = true;
	    break;
	}
    }
    if (!overlap) {
	return false;
    }

    // Add unobserved stochastic children of candidate node to set
    for (unsigned int i = 0; i < stoch_nodes.size(); ++i) {
	if (!stoch_nodes[i]->isObserved()) {
	    stochastic_children.insert(stoch_nodes[i]);
	}
    }
    
    return true;
}


namespace cut {
    

    CutFactory::~CutFactory()
    {}
    
    Sampler * 
    CutFactory::makeSampler(set<StochasticNode*> const &nodes, 
			    Graph const &graph) const
    {
	/* 
	   Find cut nodes 
	*/
	vector<StochasticNode*> candidate_nodes;
	for (set<StochasticNode*>::const_iterator p = nodes.begin();
	     p != nodes.end(); ++p)
	{
	    if ((*p)->distribution()->name() == "dcut") {
		checkChildren(*p, nodes, graph);
		candidate_nodes.push_back(*p);
	    }
	}
	if (candidate_nodes.empty())
	    return 0;

	for (unsigned int i = 0; i < candidate_nodes.size(); ++i) {

	    set<StochasticNode const*> stochastic_children
		= getStochasticChildren(candidate_nodes[i], graph);

	    //Find a joint model.
	    unsigned int nchildren;
	    vector<bool> resolved(candidate_nodes.size(), false);
	    
	    vector<StochasticNode*> sample_nodes(1, candidate_nodes[0]);
	    
	    do {
		nchildren = stochastic_children.size();
		
		for (unsigned int j = i+1; j < candidate_nodes.size(); ++j) {
		    
		    if (!resolved[j]) {
			resolved[j] = aggregateCut(candidate_nodes[j], 
						   sample_nodes, 
						   stochastic_children, 
						   graph);
		    }
		}
	    } while (nchildren < stochastic_children.size());
	 
	    if (Sampler::canSample(sample_nodes, graph)) {
		return new CutSampler(sample_nodes, graph);
	    }
	}

	return 0;
}

