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

static bool isLink(DeterministicNode const *dnode, GraphMarks const &marks)
{
    if (dnode->parents().size() != 1)
	return false; //Link node can only have a single parent

    if (marks.mark(dnode->parents()[0]) == MARK_FALSE) 
	return false; //That parent must belong to the closed class
    
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
			NodeCheckFn fun)
{
    vector<StochasticNode const *> stoch_nodes;
    vector<DeterministicNode *> dtrm_nodes;
    Sampler::classifyChildren(snodes, graph, stoch_nodes, dtrm_nodes);

    GraphMarks marks(graph);
    for (unsigned int i = 0; i < snodes.size(); ++i) {
	//Stochastic nodes are trivial functions of themselves
	marks.mark(snodes[i], MARK_TRUE);
    }
    
    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {
	if ((dtrm_nodes[j]->*fun)(marks, fixed)) {	
	    marks.mark(dtrm_nodes[j], MARK_TRUE);
	}
	else if (link && isLink(dtrm_nodes[j], marks)) {
	    //Allow link functions
	    marks.mark(dtrm_nodes[j], MARK_FALSE);
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
    return checkClosed(snodes, graph, fixed, link,
		       &DeterministicNode::isLinear);
}

bool checkScale(StochasticNode* snode, Graph const &graph, bool fixed)
{
    return checkClosed(vector<StochasticNode *>(1, snode), graph, fixed, 
		       false, &DeterministicNode::isScale);
}

bool checkPower(StochasticNode* snode, Graph const &graph, bool fixed)
{
    return checkClosed(vector<StochasticNode *>(1, snode), graph, fixed,
		       false, &DeterministicNode::isPower);
}

