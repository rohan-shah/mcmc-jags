#include <sampler/SingletonFactory.h>
#include <graph/StochasticNode.h>
#include <sampler/Sampler.h>

using std::vector;
using std::set;

vector<Sampler *>
SingletonFactory::makeSamplers(set<StochasticNode*> const &nodes, 
			       Graph const &graph) const
{
    vector<Sampler *> samplers;
    for (set<StochasticNode*>::iterator p = nodes.begin();p != nodes.end(); ++p)
    {
	if (canSample(*p, graph)) {
	    samplers.push_back(makeSampler(*p, graph));
	}
    }
    return samplers;
}
