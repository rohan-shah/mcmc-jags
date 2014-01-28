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
	Graph::iterator p = find(const_cast<Node*>(node));
	if (p != end() && *p != node) throw logic_error("Node not uniquely defined by its index in Graph");
	return p != end();
	//return find(const_cast<Node*>(node)) != end();
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


static bool childInSet(Node *node, set<Node*, ltNode> const &S)
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


void Graph::getSortedNodes(set<Node*, ltNode> &S, vector<Node*> &sortednodes) 
{
    //Return a vector of nodes whose ordering follows the partial
    //ordering of the set.  If a is after b then there is never a
    //path from a to b.

    if (!sortednodes.empty()) {
	throw logic_error("vector not empty in getSortedNodes");
    }

    sortednodes.reserve(S.size());

    while (!S.empty()) {

	bool loopcheck = false;

	set<Node*, ltNode>::iterator i = S.begin();
	while (i != S.end()) {
	    if (childInSet(*i, S)) {
		++i;
	    }
	    else {
		loopcheck = true;
		sortednodes.push_back(*i);
		set<Node*, ltNode>::iterator j = i;
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

/*
//Recursively search for path between two nodes in a set
static bool pathTo(Node *node1, Node *node2, set<Node*, ltNode> const &S)
{
    if (node1 == node2)
	return true;

    for (set<StochasticNode *>::const_iterator j = node1->stochasticChildren()->begin(); 
	 j != node1->stochasticChildren()->end(); ++j) 
    {
	set<Node*, ltNode>::const_iterator p = S.find(*j);
	if (p != S.end()) {
	    if (pathTo(*p, node2, S)) return true;
	}
    }

    for (set<DeterministicNode *>::const_iterator j = node1->deterministicChildren()->begin(); 
	 j != node1->deterministicChildren()->end(); ++j) 
    {
	set<Node*, ltNode>::const_iterator p = S.find(*j);
	if (p != S.end()) {
	    if (pathTo(*p, node2, S)) return true;
	}
    }
    return false;
}
*/

//FIXME: This should be redundant now
void Graph::getSortedNodes(vector<Node*> &sortednodes) const
{
    set<Node*, ltNode> S = *this;
    getSortedNodes(S, sortednodes);
    /*
    //Yes this is slow but its for debugging purposes
    for(unsigned int i = 0; i < sortednodes.size(); ++i) {
	for (unsigned int j = i+1; j < sortednodes.size(); ++j) {
	    if (sortednodes[j]->index() == sortednodes[i]->index()) {
		// This should never happen
		throw logic_error("Non-unique index in Graph::getSortedNodes");
	    }
	    if (sortednodes[j]->index() < sortednodes[i]->index()) {
		// We can have two nodes out of order, but only if there is no path between them 
		if (pathTo(sortednodes[i], sortednodes[j], *this)) {
		    throw logic_error("Graph::getSortedNodes out of order");
		}
		if (pathTo(sortednodes[j], sortednodes[i], *this)) {
		    throw logic_error("Graph::getSortedNodes out of order");
		}
	    }
	}
    }
    */

}


}
