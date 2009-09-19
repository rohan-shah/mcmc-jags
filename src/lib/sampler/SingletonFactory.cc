#include <sampler/SingletonFactory.h>
#include <graph/StochasticNode.h>
#include <sampler/Sampler.h>

using std::vector;
using std::set;

Sampler *
SingletonFactory::makeSampler(StochasticNodeSet const &nodes, 
			      Graph const &graph) const
{
    for (StochasticNodeSet::iterator p = nodes.begin();	 p != nodes.end(); ++p)
    {
	if (canSample(*p, graph)) {
	    return makeSampler(*p, graph);
	}
    }
    return 0;
}
