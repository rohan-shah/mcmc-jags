#include <config.h>
#include <sampler/Linear.h>
#include <sampler/Sampler.h>
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

/* 
   Check that deterministic descendants of a set of stochastic nodes
   belong to a given class of functions that are closed under composition. 
*/
static bool checkClosed(vector<StochasticNode*> const &snodes, 
			Graph const &graph,
			bool fixed, 
			bool link,
			ClosedFuncClass fun) //FIXME
{
    vector<StochasticNode const *> stoch_nodes;
    vector<DeterministicNode *> dtrm_nodes;
    Sampler::classifyChildren(snodes, graph, stoch_nodes, dtrm_nodes);

    set<Node const*> ancestors;
    for (unsigned int i = 0; i < snodes.size(); ++i) {
	//Stochastic nodes are trivial functions of themselves
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
	if (dtrm_nodes[j]->isClosed(ancestors, fun, fixed)) {
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

bool checkLinear(vector<StochasticNode*> const &snodes, 
		 Graph const &graph,
		 bool fixed, bool link)
{
    return checkClosed(snodes, graph, fixed, link, DNODE_LINEAR);
}

bool checkScale(StochasticNode* snode, Graph const &graph, bool fixed)
{
    return checkClosed(vector<StochasticNode *>(1, snode), graph, fixed, 
		       false, DNODE_SCALE);
}

bool checkScaleMix(StochasticNode* snode, Graph const &graph)
{
    return checkClosed(vector<StochasticNode *>(1, snode), graph, false,
                       false, DNODE_SCALE_MIX);
}

bool checkPower(StochasticNode* snode, Graph const &graph, bool fixed)
{
    return checkClosed(vector<StochasticNode *>(1, snode), graph, fixed,
		       false, DNODE_POWER);
}

