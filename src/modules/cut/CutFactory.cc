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

/* Get unobserved stochastic children */
static set<StochasticNode const*> getUSC(StochasticNode *node, 
					 Graph const &graph)
{
    set<StochasticNode const*> children;
    Sampler::getStochasticChildren(vector<StochasticNode*>(1, node),
				   graph, children);
    for (set<StochasticNode const*>::iterator p = children.begin();
	 p != children.end(); ++p)
    {
	if ((*p)->isObserved()) {
	    children.erase(p++);
	}
    }
    return children;
}

static bool checkChildren(StochasticNode *snode, Graph const &graph, 
			  set<StochasticNode*> &sample_nodes)
{
    //Check to see if cutnode has any unobserved children that have
    //already been assigned a sampler, and are hence absent from the
    //set of sample nodes.

    set<StochasticNode const*> children = getUSC(snode, graph);
    
    for (set<StochasticNode const*>::const_iterator p = children.begin();
	 p != children.end(); ++p)
    {
	if (sample_nodes.count(*p) == 0) {
	    return false;
	}
    }

    return true;
}

/*
  Aggregates cut nodes with common unobserved children into a joint model. 
  
  Modifies the arguments in place and returns true the status of
  the candidate node can be resolved.
*/
static bool aggregateCut(StochasticNode *candidate_node,
			 set<StochasticNode const*> &stochastic_children,
			 Graph const &graph)
{

    // Check that there is some overlap in unobserved stochastic
    // children between candidate node and current set.  If no, then
    // we have to defer judgement.

    set<StochasticNode const*> candidate_children =
	getUSC(candidate_node, graph);

    bool overlap = false;
    for (unsigned int i = 0; i < candidate_children.size(); ++i) {
	if (stochastic_children.count(candidate_children[i]) > 0) {
	    overlap = true;
	    break;
	}
    }
    if (!overlap) {
	return false;
    }

    // Add unobserved stochastic children of candidate node to set
    for (unsigned int i = 0; i < candidate_children.size(); ++i) {
	stochastic_children.insert(stoch_nodes[i]);
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
		if (!checkChildren(*p, nodes, graph))
		    return 0;
		candidate_nodes.push_back(*p);
	    }
	}
	if (candidate_nodes.empty())
	    return 0;

	for (unsigned int i = 0; i < candidate_nodes.size(); ++i) {

	    set<StochasticNode const*> stochastic_children
		= getUSC(candidate_nodes[i], graph);

	    //Find a joint model.
	    unsigned int nchildren;
	    vector<bool> resolved(candidate_nodes.size(), false);
	    
	    resolved[i] = true;
	    vector<StochasticNode*> sample_nodes(1, candidate_nodes[i]);
	    
	    do {
		nchildren = stochastic_children.size();
		
		for (unsigned int j = 0; j < candidate_nodes.size(); ++j) {
		    
		    if (!resolved[j]) {
			resolved[j] = aggregateCut(candidate_nodes[j], 
						   sample_nodes, 
						   stochastic_children, 
						   graph);
		    }
		}
	    } while (nchildren < stochastic_children.size());
	 
	    if (CutSampler::canSample(sample_nodes, graph)) {
		return new CutSampler(sample_nodes, graph);
	    }
	}

	return 0;
}

