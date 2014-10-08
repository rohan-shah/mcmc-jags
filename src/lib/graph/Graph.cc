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

}
