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

/**
 * Two nodes are considered equal if they have the same address, or if
 * they are both constant and have the same value (within numerical
 * tolerance), or if they are deterministic, fixed nodes with the same
 * value.
*/
bool equal(Node const *node1, Node const *node2)
{
  if (node1 == node2) {
    return true;
  }
  else if (node1->isObserved() && node2->isObserved() &&
           node1->length() == node2->length()) {
    unsigned long n = node1->length();
    double const *value1 = node1->value(0);
    double const *value2 = node2->value(0);
    for (unsigned long i = 0; i < n; ++i) {
      if (!equal(value1[i],value2[i])) {
        return false;
      }
    }
    return true;
  }
  else {
    return false;
  }
}


static bool equal(vector<Node const*> const &v1, vector<Node const*> const &v2)
{
  unsigned long n1 = v1.size();
  unsigned long n2 = v1.size();
  if (n1 != n2) {
    return false;
  }
  else {
    for (unsigned int i = 0; i < n1; ++i) {
      if (!equal(v1[i], v2[i])) {
	return false;
      }
    }
    return true;
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
    //Order by parameter length
    unsigned long n1 = arg1.second.size();
    unsigned long n2 = arg2.second.size();
    if (n1 < n2) {
      return true;
    }
    else if (n1 > n2) {
      return false;
    }
    else {
      //Same parameter length. 
      //Check for equality
      if (equal(arg1.second, arg2.second)) {
	return false;
      }
      else {
	//Order by address in memory
	for (unsigned int i = 0; i < n1; ++i) {
	  if (arg1.second[i] < arg2.second[i]) {
	    return true;
	  }
	  else if (arg1.second[i] > arg2.second[i]) {
	    return false;
	  }
	}
	return false;
      }
    }
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
