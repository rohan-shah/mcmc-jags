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
    : _graph(graph)
{
    set<Node*> const &nodes = graph.nodes();
    for (set<Node*>::const_iterator i = nodes.begin(); i != nodes.end(); ++i) {
	_marks[*i] = 0;
    }
}

GraphMarks::~GraphMarks()
{}

Graph const &GraphMarks::graph() const
{
    return _graph;
}

void GraphMarks::mark(Node const *node, int m)
{
    map<Node const*, int>::iterator i = _marks.find(node);
    if (i == _marks.end()) {
	throw logic_error("Attempt to set mark of node not in graph");
    }
    else {
	i->second = m;
    }
}

int GraphMarks::mark(Node const *node) const
{
    map<Node const*, int>::const_iterator i = _marks.find(node);
    if (i == _marks.end()) {
	throw logic_error("Attempt to get mark of node not in Graph");
    }
    return i->second;
}

void GraphMarks::markAll(int m)
{
    for (map<Node const*, int>::iterator i = _marks.begin();
	 i != _marks.end(); ++i) 
    {
	i->second = m;
    }
}

void GraphMarks::markParents(Node const *node, int m)
{

    if (_marks.find(node) == _marks.end()) {
	throw logic_error("Can't mark parents of node: not in Graph");
    }
    else {
	vector<Node const *> const &parents = node->parents();
	for (vector<Node const *>::const_iterator p = parents.begin(); 
	     p != parents.end(); ++p) 
	{
	    map<Node const*, int>::iterator i = _marks.find(*p);    
	    if (i != _marks.end()) {
		i->second = m;
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
    else {
	vector<Node const *> const &parents = node->parents();
	for (unsigned int j = 0; j < parents.size(); ++j) {
	    map<Node const*, int>::iterator i = _marks.find(parents[j]);
	    if (i != _marks.end()) {
		if (test(i->first)) {
		    i->second = m;
		}
		else {
		    markParents(i->first, test, m);
		}
	    }
	}
    }
}

void GraphMarks::markChildren(Node *node, int m)
{

    if (!_graph.contains(node)) {
	throw logic_error("Can't mark children of node: not in Graph");
    }
    else {
	set<Node*> const *children = node->children();
	for (set<Node*>::const_iterator p = children->begin(); 
	     p != children->end(); ++p) 
	{
	    map<Node const*, int>::iterator i = _marks.find(*p);    
	    if (i != _marks.end()) {
		i->second = m;
	    }
	}
    }
}

void GraphMarks::markChildren(Node *node, bool (*test)(Node const *), int m)
{

    if (!_graph.contains(node)) {
	throw logic_error("Can't mark children of node: not in Graph");
    }
    else {
	set<Node*> const *children = node->children();
	for (set<Node*>::const_iterator p = children->begin(); 
	     p != children->end(); ++p) 
	{
	    map<Node const*, int>::iterator i = _marks.find(*p);    
	    if (i != _marks.end()) {
		if (test(i->first)) {
		    i->second = m;
		}
		else {
		    markChildren(*p, test, m);
		}
	    }
	}
    }
}

static void do_mark_descendants(Node *node, int m, GraphMarks *gmarks,
				GraphMarks &visited)
{
    // Recursive helper function for GraphMarks::markDescendants
    for (set<Node*>::const_iterator i = node->children()->begin();
	 i != node->children()->end(); ++i) 
    {
	Node const *child = *i;
	if (visited.graph().contains(child) && visited.mark(child) == 0) {
	    visited.mark(child, 1);
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
    GraphMarks visited_nodes(_graph);
    do_mark_descendants(node, m, this, visited_nodes);
}

static void do_mark_ancestors(Node const *node, int m, GraphMarks *gmarks,
			      GraphMarks &visited)
{
    // Recursive helper function for GraphMarks::markAncestors
    for (vector<Node const*>::const_iterator i = node->parents().begin();
	 i != node->parents().end(); ++i) 
    {
	Node const *parent = *i;
	if (visited.graph().contains(parent) && visited.mark(parent) == 0) {
	    visited.mark(parent, 1);
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
       visited_nodes keeps track of previously visited nodes in order
       to avoid an infinite loop when the graph has a directed cycle.
    */
    GraphMarks visited_nodes(_graph);
    do_mark_ancestors(node, m, this, visited_nodes);
}
