#include <sampler/SingletonFactory.h>
#include <graph/StochasticNode.h>
#include <sampler/Sampler.h>

using std::vector;
using std::set;

void SingletonFactory::makeSampler(set<StochasticNode*> &nodes, 
				   Graph const &graph,
				   vector<Sampler*> &samplers) const
{
    for (set<StochasticNode*>::iterator p(nodes.begin()); p != nodes.end(); )
    {
	if (canSample(*p, graph)) {
	    samplers.push_back(makeSingletonSampler(*p, graph));
	    nodes.erase(p++); //Avoid invalidating the iterator
	}
	else {
	    ++p;
	}
    }
}
