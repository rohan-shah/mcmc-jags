#include <config.h>
#include <sampler/ParallelSampler.h>
#include <sampler/GraphView.h>
#include <sampler/SampleMethod.h>

#include "FiniteMethod.h"
#include "FiniteFactory.h"

#include <vector>

using std::vector;
using std::string;

namespace base {

    FiniteFactory::FiniteFactory()
	: _name("Finite")
    {}
    
    bool 
    FiniteFactory::canSample(StochasticNode *snode, Graph const &graph) const
    {
	return FiniteMethod::canSample(snode);
    }

    Sampler *  FiniteFactory::makeSampler(StochasticNode *snode,
					  Graph const &graph) const
    {
	GraphView *gv = new GraphView(snode, graph);
	unsigned int N = nchain(gv);
	vector<SampleMethod*> methods(N, 0);
	for (unsigned int ch = 0; ch < N; ++ch) {
	    methods[ch] = new FiniteMethod(gv, ch);
	}
	return new ParallelSampler(gv, methods);
    }

    string const &FiniteFactory::name() const
    {
	return _name;
    }
}
