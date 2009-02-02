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

Graph::Graph()
    : _nodes(), _stoch_nodes()
{
}


Graph::~Graph()
{
  for (set<Node*>::iterator p = _nodes.begin(); p != _nodes.end(); p++) {
    (*p)->unref();
  }
  Node::sweep();
}

void Graph::add(StochasticNode *snode)
{
    if (!snode) {
      throw invalid_argument("Attempt to add null node to graph");
    }
    if (!this->contains(snode)) {
      snode->ref();
      _nodes.insert(snode);
      _stoch_nodes.insert(snode);
    }
}

void Graph::add(Node *node)
{
  if (!node) {
    throw invalid_argument("Attempt to add null node to graph");
  }
  if (!this->contains(node)) {
    node->ref();
    _nodes.insert(node);
  }
}

void Graph::remove(StochasticNode *snode)
{
    if (this->contains(snode)) {
       _stoch_nodes.erase(snode);
       _nodes.erase(snode);
       snode->unref();
    }
    Node::sweep();
}

void Graph::remove(Node *node)
{
  if (this->contains(node)) {
      _nodes.erase(node);
      node->unref();
  }
  Node::sweep();
}

void Graph::clear()
{
    for (set<Node*>::iterator p = _nodes.begin(); p != _nodes.end(); ++p) {
	(*p)->unref();
    }
    _nodes.clear();
    Node::sweep();
}

bool Graph::contains(Node const *node) const
{
    return  _nodes.find(const_cast<Node*>(node)) != _nodes.end();
}

unsigned int Graph::size() const
{
  return _nodes.size();
}

bool Graph::isClosed() const
{
    //Determine whether any nodes in the graph have children or
    //parents outside the graph 

    for (set<Node*>::iterator i = _nodes.begin(); i != _nodes.end(); i++) {
    
	// Check parents
	vector<Node const *> const &parents = (*i)->parents();
	for (vector<Node const *>::const_iterator j = parents.begin(); 
	     j != parents.end(); j++) 
	{
	    if (!this->contains(*j)) {
		return false;
	    }
	}

	// Check children
	set<StochasticNode*> const *sch = (*i)->stochasticChildren();
	for (set<StochasticNode*>::iterator k = sch->begin(); k != sch->end(); k++)
	{
	    if (!this->contains(*k)) {
		return false;
	    }
	}

	set<DeterministicNode*> const *dch = (*i)->deterministicChildren();
	for (set<DeterministicNode*>::iterator k = dch->begin(); k != dch->end(); k++)
	{
	    if (!this->contains(*k)) {
		return false;
	    }
	}
    }
    return true;
}

/*
bool Graph::isConnected() const
{
  GraphMarks marks(*this);
 
  // Start by taking an arbitrary node 
  set<Node*>::const_iterator i = _nodes.begin();
  Node *anode = *i;

  // Mark the node, its parents and ancestors 
  marks.mark(anode, 1);
  marks.markAncestors(anode, 1);
  marks.markDescendants(anode, 1);
  
  // If the graph is connected, all the other nodes will be marked 
  for (++i; i != _nodes.end(); ++i) {
    if (marks.mark(*i) == 0) {
      return false;
    }
  }
  return true;
}
*/

/*
bool Graph::isTree()
{
     Determine whether a graph is a tree
     All nodes in the graph have exactly one parent in the graph,
     except for the root node which has none. The root node is unique
  
  bool foundroot = false;

  for (set<Node*>::iterator i = _nodes.begin(); 
       i != _nodes.end(); i++) {
    
    int nparents = 0;
    vector<Node*> const &parents = (*i)->parents();
    for (vector<Node*>::const_iterator p = parents.begin(); 
	 p != parents.end(); ++i) {
      
      if (contains(*p))
	nparents++;
    }
      
    switch (nparents) {
    case 0:
      if (foundroot)
	return false;
      else 
	foundroot = true;
      break;
    case 1:
      break;
    default:
      return false;
      break;
    }
  }
  return true;
}
*/

/*
static bool findDescendant(Node const *node, Node const *target, 
			   Graph const *graph, GraphMarks &marks)
{
  //Determine whether "target" is a descendent of "node" within the graph,
  //   excluding nodes that are already marked.

  if (marks.mark(node) == 1)
    return false;

  for (set<Node*>::const_iterator i = node->children().begin(); 
       i != node->children().end(); i++) 
    {
      if (graph->contains(*i)) {
	if (*i == target)
	  return true;
	if (findDescendant(*i, target, graph, marks))
	  return true;
      }
    }

  marks.mark(node, 1);
  return false;
}

bool Graph::hasCycle()
{
  GraphMarks marks(*this);
  for (set<Node*>::const_iterator p = _nodes.begin(); p != _nodes.end(); ++p) 
    {
      if (findDescendant(*p, *p, this, marks))
	return true;
    }
  return false;
}
*/

/*
bool Graph::hasCycle() const
{
  //   Recursively grows a an acyclic graph G, consisting of marked
  //   nodes. If G grows to the whole graph then it is acyclic.
  //
  //   We start with nodes that have no children. On each iteration
  //   we add nodes whose children are all in G. Adding such nodes
  //   cannot create a cycle.

  GraphMarks marks(*this);
  while (true) {
    long marked = 0;
    long unmarked = 0;
    for (set<Node*>::const_iterator p = _nodes.begin(); p != _nodes.end();
	 ++p)
      {
	if (marks.mark(*p) == 0) {
	  bool can_mark = true;
	  set<Node*> const *children = (*p)->children();
	  for(set<Node*>::const_iterator c = children->begin();
	      c != children->end(); ++c) 
	    {
	      if (marks.mark(*c) == 0) {
		can_mark = false;
		break;
	      }
	    }
	  if (can_mark) {
	    marks.mark(*p,1);
	    ++marked;
	  }
	  else {
	    ++unmarked;
	  }
	}
      }
    if (marked == 0) {
      return (unmarked > 0);
    }
  }
}
*/

set<StochasticNode*> const &Graph::stochasticNodes() const
{
    return _stoch_nodes;
}

set<Node*> const &Graph::nodes() const
{
    return _nodes;
}

void Graph::getNodes(vector<Node*> &nodes) const
{
  for (set<Node*>::iterator p = _nodes.begin();
       p != _nodes.end(); p++)
    {
      nodes.push_back(*p);
    }
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
    set<Node*> S = _nodes;
    getSortedNodes(S, sortednodes);
}
