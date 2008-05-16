#include <config.h>
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/Node.h>

#include <vector>
#include <set>
#include <stdexcept>

using std::map;
using std::vector;
using std::set;
using std::logic_error;

GraphMarks::GraphMarks(Graph const &graph)
    : _graph(graph), _default_mark(0)
{
}

GraphMarks::~GraphMarks()
{}

Graph const &GraphMarks::graph() const
{
    return _graph;
}

void GraphMarks::mark(Node const *node, int m)
{
    if (!_graph.contains(node)) {
	throw logic_error("Attempt to set mark of node not in graph");
    }
    _marks[node] = m;
}

int GraphMarks::mark(Node const *node) const
{
    if (!_graph.contains(node)) {
	throw logic_error("Attempt to get mark of node not in Graph");	    
    }
    
    map<Node const*, int>::const_iterator i = _marks.find(node);
    if (i == _marks.end()) {
	return _default_mark;
    }
    else {
	return i->second;
    }
}


void GraphMarks::markAll(int m)
{
    _marks.clear();
    _default_mark = m;
}

void GraphMarks::markParents(Node const *node, int m)
{
    if (!_graph.contains(node)) {
	throw logic_error("Can't mark parents of node: not in Graph");
    }
    else {
	vector<Node const *> const &parents = node->parents();
	for (vector<Node const *>::const_iterator p = parents.begin(); 
	     p != parents.end(); ++p) 
	{
	    if (_graph.contains(*p)) {
		_marks[*p] = m;
	    }
	}
    }
}

void 
GraphMarks::markParents(Node const *node, bool (*test)(Node const *), int m)
{

    if (!_graph.contains(node)) {
	throw logic_error("Can't mark parents of node: not in Graph");
    }

    vector<Node const *> const &parents = node->parents();
    for (vector<Node const*>::const_iterator p = parents.begin(); 
	 p != parents.end(); ++p)
    {
	Node const *parent = *p;
	if (_graph.contains(parent)) {
	    if (test(parent)) {
		_marks[parent] = m;
	    }
	    else {
		markParents(parent, test, m);
	    }
	}
    }
}

void GraphMarks::markChildren(Node *node, int m)
{

    if (!_graph.contains(node)) {
	throw logic_error("Can't mark children of node: not in Graph");
    }

    set<Node*> const *children = node->children();
    for (set<Node*>::const_iterator p = children->begin(); 
	 p != children->end(); ++p) 
    {
	if (_graph.contains(*p)) {
	    _marks[*p] = m;
	}
    }
}

void GraphMarks::markChildren(Node *node, bool (*test)(Node const *), int m)
{

    if (!_graph.contains(node)) {
	throw logic_error("Can't mark children of node: not in Graph");
    }

    set<Node*> const *children = node->children();
    for (set<Node*>::const_iterator p = children->begin(); 
	 p != children->end(); ++p) 
    {
	Node *child = *p;
	if (_graph.contains(child)) {
		if (test(child)) {
		    _marks[child] = m;
		}
		else {
		    markChildren(child, test, m);
		}
	}
    }
}

static void do_mark_descendants(Node *node, int m, GraphMarks *gmarks,
				set<Node const*> &visited)
{
    // Recursive helper function for GraphMarks::markDescendants
    for (set<Node*>::const_iterator i = node->children()->begin();
	 i != node->children()->end(); ++i) 
    {
	Node const *child = *i;
	if (gmarks->graph().contains(child) && visited.count(child) == 0) {
	    visited.insert(child);
	    gmarks->mark(child, m);
	    do_mark_descendants(*i, m, gmarks, visited);
	}
    }
}

void GraphMarks::markDescendants(Node *node, int m)
{
    if (!_graph.contains(node)) {
	throw logic_error("Can't mark children of node: not in Graph");
    }
    /* 
       visited_nodes keeps track of previously visited nodes in order
       to avoid an infinite loop when the graph has a directed cycle.
    */
    set<Node const*> visited_nodes;
    do_mark_descendants(node, m, this, visited_nodes);
}

static void do_mark_ancestors(Node const *node, int m, GraphMarks *gmarks,
			      set<Node const*> &visited)
{
    // Recursive helper function for GraphMarks::markAncestors
    for (vector<Node const*>::const_iterator i = node->parents().begin();
	 i != node->parents().end(); ++i) 
    {
	Node const *parent = *i;
	if (visited.count(parent) == 0 && gmarks->graph().contains(parent)) {
	    visited.insert(parent);
	    gmarks->mark(parent, m);
	    do_mark_ancestors(parent, m, gmarks, visited);
	}
    }
}

void GraphMarks::markAncestors(Node const *node, int m)
{
    if (!_graph.contains(node)) {
	throw logic_error("Can't mark children of node: not in Graph");
    }
    /* 
       visited_nodes keeps track of previously visited nodes for efficiency.
       This also protects against directed cycles.
    */
    set<Node const*> visited_nodes;
    do_mark_ancestors(node, m, this, visited_nodes);
}
