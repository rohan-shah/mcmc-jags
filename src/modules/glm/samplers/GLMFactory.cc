#include <config.h>

#include "GLMFactory.h"
#include "GLMSampler.h"

#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <graph/LinkNode.h>
#include <distribution/Distribution.h>
#include <sampler/ParallelSampler.h>
#include <sampler/Linear.h>
#include <sampler/Updater.h>

#include <set>
#include <map>
#include <algorithm>

using std::set;
using std::vector;
using std::map;
using std::stable_sort;
using std::string;


/*
  Aggregates candidate Nodes into a joint linear model. 
  
  Although each candidate node has a conjugate normal distribution,
  together they may not form a valid linear model. We therefore test
  the validity of the joint linear model before aggregating.
  
  Modifies the arguments in place and returns true if the status of
  the candidate node can be resolved.
*/
static bool aggregateLinear(StochasticNode *candidate_node,
			    vector<StochasticNode*> &sample_nodes,
			    set<StochasticNode const *> &stochastic_children,
			    Graph const &graph)
{

    // Check that there is some overlap in stochastic children between
    // candidate node and current set.  If no, then we have to defer
    // judgement.

    Updater updater(candidate_node, graph);
    vector<StochasticNode const*> const &candidate_children = 
	updater.stochasticChildren();

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

    // Check linearity of joint model
    
    sample_nodes.push_back(candidate_node);
    Updater updater2(sample_nodes, graph);
    if (!checkLinear(&updater2, false, true)) {
	sample_nodes.pop_back();
	return true;
    }

    // Add stochastic children of candidate node to set
    stochastic_children.insert(candidate_children.begin(), candidate_children.end());
    
    return true;
}

//Utility function
static set<StochasticNode const *>
getStochasticChildren(StochasticNode *snode, Graph const &graph)
{
    Updater updater(snode, graph);
    set<StochasticNode const *> ans;
    ans.insert(updater.stochasticChildren().begin(),
	       updater.stochasticChildren().end());
    return ans;
}

struct less_node {
    /* 
       Comparison operator for candidate nodes which sorts them in
       reverse order according to the supplied map.
    */
    map<Node const *, unsigned int > const & _map;

    less_node(map<Node const *, unsigned int > const &map)
	: _map(map) {};

    bool operator()(Node const *x, Node const *y) const {
	return _map.find(x)->second > _map.find(y)->second;
    };
};


namespace glm {
    
    unsigned int 
    GLMFactory::canSample(StochasticNode *snode, Graph const &graph) const
    {
	/*
	  Check whether whether an individual node can be sampled 

	  Returns the number of stochastic children if successful,
	  otherwise zero.
	*/

	string dname = snode->distribution()->name();
	if (dname != "dnorm" && dname != "dmnorm")
	    return 0;
    

	Updater updater(snode, graph);
	vector<StochasticNode const*> const &stoch_nodes = 
	    updater.stochasticChildren();
	vector<DeterministicNode *> const &dtrm_nodes =
	    updater.deterministicChildren();

	/* 
	   Create a set of nodes containing snode and its descendants
	   for the checks below.
	*/
	set<Node const*> paramset;
	paramset.insert(snode);
	paramset.insert(stoch_nodes.begin(), stoch_nodes.end());
	paramset.insert(dtrm_nodes.begin(), dtrm_nodes.end());

	// Check stochastic children
	bool have_link = false;
	for (unsigned int i = 0; i < stoch_nodes.size(); ++i) {
	    if (isBounded(stoch_nodes[i])) {
		return 0; //Truncated outcome variable
	    }
	    if (!checkOutcome(stoch_nodes[i])) {
		return 0; //Invalid outcome distribution
	    }
	    vector<Node const *> const &param = stoch_nodes[i]->parents();
	    //Check for link functions
	    LinkNode const *lnode = dynamic_cast<LinkNode const*>(param[0]);
	    if (lnode) {
		have_link = true;
		if (!checkLink(lnode->link()))
		    return 0;
	    }
	    //Check that other parameters do not depend on snode	    
	    for (unsigned int j = 1; j < param.size(); ++j) {
		if (paramset.count(param[j])) {
		    return 0;
		}
	    }
	}

	// Check linearity of deterministic descendants
	if (!checkLinear(&updater, false, have_link))
	    return 0;

	return stoch_nodes.size();
    }

    GLMFactory::~GLMFactory()
    {}
    
    Sampler * 
    GLMFactory::makeSampler(set<StochasticNode*> const &nodes, 
			    Graph const &graph) const
    {
	/* 
	   Find candidate nodes that could be in a linear model.
	   Keep track of the number of stochastic children
	*/
	vector<StochasticNode*> candidate_nodes;
	map<Node const*, unsigned int> candidate_map;
	for (set<StochasticNode*>::const_iterator p = nodes.begin();
	     p != nodes.end(); ++p)
	{
	    unsigned int size = canSample(*p, graph);
	    if (size > 0) {
		candidate_nodes.push_back(*p);
                candidate_map[*p] = size;
	    }
	}
	if (candidate_nodes.empty())
	    return 0;

	//Sort candidates in order of decreasing number of stochastic children
	stable_sort(candidate_nodes.begin(), candidate_nodes.end(),
		    less_node(candidate_map));

	for (unsigned int i = 0; i < candidate_nodes.size(); ++i) {
	    
	    vector<StochasticNode*> sample_nodes(1, candidate_nodes[i]);
	    set<StochasticNode const *> stochastic_children
		= getStochasticChildren(candidate_nodes[i], graph);

	    //Find a joint linear model.
	    unsigned int nchildren;
	    vector<bool> resolved(candidate_nodes.size(), false);
	    do {
		nchildren = stochastic_children.size();

		for (unsigned int j = i+1; j < candidate_nodes.size(); ++j) {

		    if (!resolved[j]) {
			resolved[j] = aggregateLinear(candidate_nodes[j], 
						      sample_nodes, 
						      stochastic_children, 
						      graph);
		    }
		}
	    } while (nchildren < stochastic_children.size());

	    if (sample_nodes.size() > 1) {
		//We must have at least two nodes to sample
		Updater *updater = new Updater(sample_nodes, graph);
		return new GLMSampler(updater, newMethod(updater)); 
	    }
	}
	
	return 0;
    }

}

