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
  map<LogicalPair, LogicalNode*, ltlogical>::iterator i 
      = _logicalmap.find(lpair);

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
