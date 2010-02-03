#include <config.h>
#include <model/Monitor.h>
#include <graph/StochasticNode.h>
#include <graph/Node.h>

#include <stdexcept>

using std::string;

Monitor::Monitor(string const &type, Node const *node)
    : _type(type), _node(node)
{
}

Monitor::~Monitor()
{}

Node const * Monitor::node() const
{
    return _node;
}

string const &Monitor::type() const
{
    return _type;
}
