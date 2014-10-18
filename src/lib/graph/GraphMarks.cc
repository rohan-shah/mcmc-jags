#include <config.h>
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/Node.h>

#include <vector>
#include <set>
#include <stdexcept>
#include <utility>
#include <list>

using std::map;
using std::vector;
using std::set;
using std::logic_error;
using std::pair;
using std::list;

namespace jags {


    /* 
       This is a helper class for the markAncestors member function. A
       GMIterator moves across a vector of Nodes. A dereferencing
       operator and a prefix increment operator are provided so that
       it behaves like an STL iterator.

       Unlike an STL iterator, however, a GMIterator stores its end
       position.  The atEnd member function can be used to test
       whether the iterator has moved beyond the last element of the
       vector.
    */
    class GMIterator {
	vector<Node const*>::const_iterator _begin, _end;
    public:
	GMIterator(vector<Node const*> const &v)
	    : _begin(v.begin()), _end(v.end()) {}
	inline Node const * operator*() const { return *_begin; }
	inline bool atEnd() const { return _begin == _end; }
	inline void operator++() { ++_begin; }
    };

GraphMarks::GraphMarks(Graph const &graph)
    : _graph(graph)
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
    if (m == 0) {
	_marks.erase(node);
    }
    else {
	_marks[node] = m;
    }
}

int GraphMarks::mark(Node const *node) const
{
    if (!_graph.contains(node)) {
	throw logic_error("Attempt to get mark of node not in Graph");	    
    }
    
    map<Node const*, int>::const_iterator i = _marks.find(node);
    if (i == _marks.end()) {
	return 0;
    }
    else {
	return i->second;
    }
}

void GraphMarks::clear()
{
    _marks.clear();
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

//FIXME
//Used by MixtureSampler factory
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

void GraphMarks::markAncestors(vector<Node const *> const &nodes, int m)
{
    set<Node const*> visited; //visited nodes
    vector<Node const*> ancestors; //ancestor nodes
    
    /* 
       Do a depth-first search of the graph to find all the ancestors
       of the given Nodes in the graph. The set "visited" keeps track
       of previously visited nodes for efficiency. Ancestors are
       pushed back on to the vector "ancestors" in the order they are
       found.

       We could do this with a recursive helper function, but it is
       safer to iterate. So we keep our own stack of GMIterators to
       record the current position on the graph.

       The GMIterator class is defined above.
    */
  
    vector<GMIterator> stack;
    stack.push_back(GMIterator(nodes));

    while (!stack.empty()) {

	for (GMIterator &p = stack.back(); !p.atEnd(); ++p) {
	    if (visited.count(*p) == 0 && _graph.contains(*p)) {
		visited.insert(*p);
		ancestors.push_back(*p);
		stack.push_back(GMIterator((*p)->parents()));
		break;
	    }
	}
	
	if (stack.back().atEnd()) {
	    stack.pop_back();
	}
    }

    /* Now set the marks of all ancestors */
    for(vector<Node const*>::const_iterator p = ancestors.begin();
	p != ancestors.end(); ++p)
    {
	if (m == 0) {
	    _marks.erase(*p);
	}
	else {
	    _marks[*p] = m;
	}
    }

}

} //namespace jags
