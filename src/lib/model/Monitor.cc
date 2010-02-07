#include <config.h>
#include <model/Monitor.h>
#include <graph/StochasticNode.h>
#include <graph/Node.h>
#include <util/dim.h>

#include <stdexcept>

using std::string;
using std::vector;
using std::logic_error;

Monitor::Monitor(string const &type, vector<Node const *> const &nodes)
    : _type(type), _nodes(nodes)
{
}

Monitor::Monitor(string const &type, Node const *node)
    : _type(type), _nodes(vector<Node const*>(1,node))
{
}

Monitor::~Monitor()
{}

string const &Monitor::type() const
{
    return _type;
}

vector<Node const*> const &Monitor::nodes() const
{
    return _nodes;
}

string const &Monitor::name() const
{
    return _name;
}

void Monitor::setName(std::string const &name)
{
    _name = name;
}

vector<string> const &Monitor::elementNames() const
{
    return _elt_names;
}

void Monitor::setElementNames(vector<string> const &names)
{
/*
  //debuggin
    if (names.size() != product(dim1())) {
	throw logic_error("Invalid length is Monitor::setElementNames");
    }
*/
    _elt_names = names;
}
