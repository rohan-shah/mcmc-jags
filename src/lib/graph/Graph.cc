#include <config.h>
#include <graph/Graph.h>
#include <graph/GraphMarks.h>
#include <graph/Node.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>

#include <stdexcept>
#include <vector>
#include <set>
#include <algorithm>

using std::vector;
using std::set;
using std::invalid_argument;
using std::logic_error;
using std::reverse;

namespace jags {

    Graph::Graph() {}

    bool Graph::contains(Node const *node) const
    {
	return find(const_cast<Node*>(node)) != end();
    }

    bool Graph::isClosed() const
    {
	//Determine whether any nodes in the graph have children or
	//parents outside the graph 

	for (iterator i = begin(); i != end(); i++) {
    
	    // Check parents
	    vector<Node const *> const &parents = (*i)->parents();
	    for (vector<Node const *>::const_iterator j = parents.begin(); 
		 j != parents.end(); j++) 
	    {
		if (!contains(*j)) return false;
	    }

	    // Check children
	    set<StochasticNode*> const *sch = (*i)->stochasticChildren();
	    for (set<StochasticNode*>::iterator k = sch->begin(); k != sch->end(); k++)
	    {
		if (!contains(*k)) return false;
	    }

	    set<DeterministicNode*> const *dch = (*i)->deterministicChildren();
	    for (set<DeterministicNode*>::iterator k = dch->begin(); k != dch->end(); k++)
	    {
		if (!contains(*k)) return false;
	    }
	}
	return true;
    }

/* Helper function for Graph::getSortedNodes. Returns true
   if node has any children in set S */
static bool childInSet(Node *node, set<Node*> const &S)
{
    for (set<StochasticNode *>::const_iterator j = node->stochasticChildren()->begin(); 
	 j != node->stochasticChildren()->end(); ++j) 
    {
	if (S.count(*j)) {
	    return true;
	}
    }
    for (set<DeterministicNode *>::const_iterator j = node->deterministicChildren()->begin(); 
	 j != node->deterministicChildren()->end(); ++j) 
    {
	if (S.count(*j)) {
	    return true;
	}
    }
    return false;
}


void Graph::getSortedNodes(set<Node*> &S, vector<Node*> &sortednodes) 
{
    /* 
       Return a vector of nodes whose ordering follows the partial
       ordering of the set.  If a is after b then there is never a
       path from a to b.
    */

    if (!sortednodes.empty()) {
	throw logic_error("vector not empty in getSortedNodes");
    }

    sortednodes.reserve(S.size());

    while (!S.empty()) {

	bool loopcheck = false;

	set<Node*>::iterator i = S.begin();
	while (i != S.end()) {
	    if (childInSet(*i, S)) {
		++i;
	    }
	    else {
		loopcheck = true;
		sortednodes.push_back(*i);
		set<Node*>::iterator j = i;
		++i;
		S.erase(j);
	    }
	}
	
	if (!loopcheck) {
	    //We did not add any nodes to sortednodes on this pass
	    throw logic_error("Failure in Graph::getSortedNodes. Directed cycle in graph");
	}
    }

    reverse(sortednodes.begin(), sortednodes.end());
}

void Graph::getSortedNodes(vector<Node*> &sortednodes) const
{
    set<Node*> S = *this;
    getSortedNodes(S, sortednodes);
}

}
