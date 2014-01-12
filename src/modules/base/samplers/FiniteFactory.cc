#include <config.h>
#include <sampler/ParallelSampler.h>
#include <sampler/SingletonGraphView.h>
#include <sampler/SampleMethod.h>

#include "FiniteMethod.h"
#include "FiniteFactory.h"

#include <vector>

using std::vector;
using std::string;

namespace jags {
namespace base {

    bool 
    FiniteFactory::canSample(StochasticNode *snode, Graph const &graph) const
    {
	return FiniteMethod::canSample(snode);
    }

    Sampler *  FiniteFactory::makeSampler(StochasticNode *snode,
					  Graph const &graph) const
    {
	SingletonGraphView *gv = new SingletonGraphView(snode, graph);
	unsigned int N = nchain(gv);
	vector<SampleMethod*> methods(N, 0);
	for (unsigned int ch = 0; ch < N; ++ch) {
	    methods[ch] = new FiniteMethod(gv, ch);
	}
	return new ParallelSampler(gv, methods);
    }

    string FiniteFactory::name() const
    {
	return "base::Finite";
    }

}}
