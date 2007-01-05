#include <config.h>
#include <compiler/LogicalFactory.h>
#include <graph/Node.h>
#include <graph/LogicalNode.h>
#include <function/Function.h>
#include <compiler/NodeFactory.h>

#include <stdexcept>

using std::pair;
using std::map;
using std::vector;
using std::invalid_argument;

#define TOL 16 * DBL_EPSILON

/* Comparison operator for Nodes */
static bool lt(Node const *node1, Node const *node2)
{
    if (node1 == node2) {
	// A node is always identical to itself
	return false; 
    }

    if (node1->isObserved() && !node2->isObserved()) {
	//Observed nodes come before unobserved nodes
	return true;
    }
    else if (!node1->isObserved() && node2->isObserved()) {
	return false;
    }
    else if (node1->isObserved() && node2->isObserved()) {
	//Observed nodes are sorted by dimension, then value
	if (node1->dim() < node2->dim()) {
	    return true;
	}
	else if (node1->dim() > node2->dim()) {
	    return false;
	}
	else {
	    //Equal length observed nodes are sorted by value, with
	    //some numerical tolerance
	    unsigned long n = node1->length();
	    double const *value1 = node1->value(0);
	    double const *value2 = node2->value(0);
	    for (unsigned long i = 0; i < n; ++i) {
		if (value1[i] < value2[i] - TOL) {
		    return true;
		}
		else if (value1[i] > value2[i] + TOL) {
		    return false;
		}
	    }
	    return false; //Numerically identical observed nodes
	}
    }
    else {
	//Unobserved nodes are sorted by address. The ordering is
	//arbitrary, but unique.
	return (node1 < node2);
    }
}

/* Comparison operator for vectors of parameters */
static bool 
lt(vector<Node const *> const &par1, vector<Node const *> const &par2)
{
    //Order by size
    if (par1.size() < par2.size()) {
	return true;
    }
    else if (par1.size() > par2.size()) {
	return false;
    }
    else {
        //Equal sized vectors: Sort by ordering of elements 
	for (unsigned int i = 0; i < par1.size(); ++i) {
	    if (lt(par1[i], par2[i])) {
		return true;
	    }
	    else if (lt(par2[i], par1[i])) {
		return false;
	    }
	}
	return false;
    }
}

bool lt(LogicalPair const &arg1, LogicalPair const &arg2)
{
    // Order first by function
    if (arg1.first < arg2.first) {
	return true;
    }
    else if (arg1.first > arg2.first) {
	return false;
    }
    else {
	//Same function. Check parameters
	return lt(arg1.second, arg2.second);
    }
}

LogicalNode*
LogicalFactory::getLogicalNode(Function const *func, 
			       vector<Node const *> const &parents,
                               Graph &graph)
{
  if (func == 0) {
    throw invalid_argument("NULL function passed to getLogicalNode");
  }

  LogicalPair lpair(func, parents);
  map<LogicalPair, LogicalNode*>::iterator i = _logicalmap.find(lpair);

  if (i != _logicalmap.end()) {
    return i->second;
  }
  else {
    // Create a new logical node
    LogicalNode *lnode = new LogicalNode(func, parents);
    _logicalmap[lpair] = lnode;
    graph.add(lnode);
    return lnode;
  }
}
