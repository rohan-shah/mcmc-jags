#include <config.h>
#include <sampler/Linear.h>
#include <sampler/Updater.h>
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <graph/LinkNode.h>
#include <graph/Node.h>

using std::vector;
using std::set;

typedef bool (DeterministicNode::*NodeCheckFn) (GraphMarks const&, bool) const;

static bool isLink(DeterministicNode const *dnode)
{
    return dynamic_cast<LinkNode const*>(dnode) != 0;
}

bool checkLinear(Updater const *updater, bool fixed, bool link)
{
    vector<StochasticNode const *> const &stoch_nodes
	= updater->stochasticChildren();
    vector<DeterministicNode *> const &dtrm_nodes
	= updater->deterministicChildren();

    set<Node const*> ancestors;
    //Sampled nodes are trivial (fixed) linear functions of themselves
    ancestors.insert(updater->nodes().begin(), updater->nodes().end());
    
    set<Node const*> stoch_node_parents;
    if (link) {
	//Put parents of stoch_nodes, which may be link functions
	for (unsigned int i = 0; i < stoch_nodes.size(); ++i) {
	    stoch_node_parents.insert(stoch_nodes[i]->parents().begin(),
				      stoch_nodes[i]->parents().end());
	}
    }
    
    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {
	if (dtrm_nodes[j]->isClosed(ancestors, DNODE_LINEAR, fixed)) {
	    ancestors.insert(dtrm_nodes[j]);
	}
	else if (link) {
	    //Allow an exception for link nodes
	    if (stoch_node_parents.count(dtrm_nodes[j]) == 0 || 
		!isLink(dtrm_nodes[j]))
	    {
		return false;
	    }
	}
	else {
	    return false;
	}
    }
    
    return true;
}

//FIXME: Deprecate?
bool checkLinear(vector<StochasticNode*> const &snodes, 
		 Graph const &graph,
		 bool fixed, bool link)
{
    Updater updater(snodes,graph);
    return checkLinear(&updater, fixed, link);
}

/*
{
    vector<StochasticNode const *> stoch_nodes;
    vector<DeterministicNode *> dtrm_nodes;
    Updater::classifyChildren(snodes, graph, stoch_nodes, dtrm_nodes);

    set<Node const*> ancestors;
    for (unsigned int i = 0; i < snodes.size(); ++i) {
	//Stochastic nodes are trivial (fixed) linear functions of themselves
	ancestors.insert(snodes[i]);
    }
    
    set<Node const*> stoch_node_parents;
    if (link) {
	//Put parents of stoch_nodes
	for (unsigned int i = 0; i < snodes.size(); ++i) {
	    Node const *p = snodes[i];
	    for (unsigned int j = 0; j < p->parents().size(); ++j) {
		stoch_node_parents.insert(p->parents()[j]);
	    }
	}
    }
    
    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {
	if (dtrm_nodes[j]->isClosed(ancestors, DNODE_LINEAR, fixed)) {
	    ancestors.insert(dtrm_nodes[j]);
	}
	else if (link) {
	    //Allow an exception for link nodes
	    if (stoch_node_parents.count(dtrm_nodes[j]) == 0 || 
		!isLink(dtrm_nodes[j]))
	    {
		return false;
	    }
	}
	else {
	    return false;
	}
    }
    
    return true;
}
*/

bool checkScale(Updater const *up, bool fixed)
{
    vector<DeterministicNode *> const &dnodes = up->deterministicChildren();
    
    set<Node const*> ancestors;
    //FIXME: valid for multiple nodes?
    ancestors.insert(up->nodes().begin(), up->nodes().end()); 

    //Start off looking for scale transformations, then fall back on
    //scale mixture transformations if fixed is false.

    bool mix = false;
    for (unsigned int j = 0; j < dnodes.size(); ++j) {

	if (!mix) {
	    if (dnodes[j]->isClosed(ancestors, DNODE_SCALE, fixed)) {
		ancestors.insert(dnodes[j]);
	    }
	    else if (!fixed) {
		mix = true;
	    }
	    else {
		return false;
	    }
	}

	if (mix) {
	    if (dnodes[j]->isClosed(ancestors, DNODE_SCALE_MIX, false)) {
		ancestors.insert(dnodes[j]);
	    }
	    else {
		return false;
	    }
	}
    }
    
    return true;
}


bool checkScale(StochasticNode* snode, Graph const &graph, bool fixed)
{
    Updater up(snode, graph);
    return checkScale(&up, fixed);
}

/*   
{
    vector<StochasticNode const *> stoch_nodes;
    vector<DeterministicNode *> dtrm_nodes;
    Updater::classifyChildren(vector<StochasticNode*>(1, snode), 
			      graph, stoch_nodes, dtrm_nodes);
    
    set<Node const*> ancestors;
    ancestors.insert(snode);

    //Start off looking for scale transformations, then fall back on
    //scale mixture transformations if fixed is false.

    bool mix = false;
    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {

	if (!mix) {
	    if (dtrm_nodes[j]->isClosed(ancestors, DNODE_SCALE, fixed)) {
		ancestors.insert(dtrm_nodes[j]);
	    }
	    else if (!fixed) {
		mix = true;
	    }
	    else {
		return false;
	    }
	}

	if (mix) {
	    if (dtrm_nodes[j]->isClosed(ancestors, DNODE_SCALE_MIX, false)) {
		ancestors.insert(dtrm_nodes[j]);
	    }
	    else {
		return false;
	    }
	}
    }
    
    return true;
}
*/

bool checkPower(Updater *updater, bool fixed)
{
    set<Node const*> ancestors;
    ancestors.insert(updater->nodes().begin(), updater->nodes().end());

    vector<DeterministicNode *> dnodes = updater->deterministicChildren();    
    for (unsigned int j = 0; j < dnodes.size(); ++j) {
	if (dnodes[j]->isClosed(ancestors, DNODE_POWER, fixed)) {
	    ancestors.insert(dnodes[j]);
	}
	else {
	    return false;
	}
    }
    
    return true;
}

bool checkPower(StochasticNode* snode, Graph const &graph, bool fixed)
{
    Updater up(snode, graph);
    return checkPower(&up, fixed);
}

/*
bool checkPower(StochasticNode* snode, Graph const &graph, bool fixed)
{
    vector<StochasticNode const *> stoch_nodes;
    vector<DeterministicNode *> dtrm_nodes;
    Updater::classifyChildren(vector<StochasticNode*>(1,snode), graph, 
			      stoch_nodes, dtrm_nodes);

    set<Node const*> ancestors;
    ancestors.insert(snode);
    
    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {
	if (dtrm_nodes[j]->isClosed(ancestors, DNODE_POWER, fixed)) {
	    ancestors.insert(dtrm_nodes[j]);
	}
	else {
	    return false;
	}
    }
    
    return true;
}
*/
