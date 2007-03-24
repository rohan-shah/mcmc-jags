#include <config.h>
#include <model/Monitor.h>
#include <graph/Node.h>

Monitor::Monitor(Node const *node)
    : _node(node)
{}

Monitor::~Monitor()
{}

Node const *Monitor::node() const
{
    return _node;
}


