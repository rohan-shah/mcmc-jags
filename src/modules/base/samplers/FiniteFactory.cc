#include <config.h>
#include <sampler/DensitySampler.h>
#include <sampler/Updater.h>
#include <sampler/DensityMethod.h>

#include "FiniteMethod.h"
#include "FiniteFactory.h"

#include <vector>

using std::vector;

namespace base {

    bool 
    FiniteFactory::canSample(StochasticNode *snode, Graph const &graph) const
    {
	return FiniteMethod::canSample(snode);
    }

    Sampler *  FiniteFactory::makeSampler(StochasticNode *snode,
					  Graph const &graph) const
    {
	Updater *updater = new Updater(snode, graph);
	unsigned int N = nchain(updater);
	vector<DensityMethod*> methods(N, 0);
	for (unsigned int ch = 0; ch < N; ++ch) {
	    methods[ch] = new FiniteMethod(updater, ch);
	}
	return new DensitySampler(updater, methods);
    }
    
}
