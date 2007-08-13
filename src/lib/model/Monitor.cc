#include <config.h>
#include <model/Monitor.h>
#include <graph/Node.h>

using std::vector;

Monitor::Monitor(vector<Node const *> const &nodes)
    : _nodes(nodes)
{}

Monitor::~Monitor()
{}

vector<Node const *> const &Monitor::nodes() const
{
    return _nodes;
}
