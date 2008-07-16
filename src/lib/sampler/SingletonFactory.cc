#include <sampler/SingletonFactory.h>
#include <graph/StochasticNode.h>
#include <sampler/Sampler.h>

using std::vector;
using std::set;

Sampler *SingletonFactory::makeSampler(set<StochasticNode*> const &nodes, 
				       Graph const &graph) const
{
    for (set<StochasticNode*>::iterator p(nodes.begin()); p != nodes.end(); ++p)
    {
	if (canSample(*p, graph)) {
	    return makeSingletonSampler(*p, graph);
	}
    }
    return 0;
}
