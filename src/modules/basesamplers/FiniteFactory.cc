#include <config.h>
#include <sarray/nainf.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <sampler/ParallelDensitySampler.h>
#include <sampler/DensityMethod.h>

#include "FiniteSampler.h"
#include "FiniteFactory.h"

#include <vector>

using std::vector;
namespace basesamplers {

    bool 
    FiniteFactory::canSample(StochasticNode * node, Graph const &graph) const
    {
	//Node must be scalar with discrete-valued distribution of full rank
	Distribution const *dist = node->distribution();
	if (!dist->isDiscreteValued())
	    return false;

	if (node->length() != 1)
	    return false;

	if (df(node) == 0)
	    return false;

	for (unsigned int ch = 0; ch < node->nchain(); ++ch) {
	    //Distribution cannot be unbounded
	    double ulimit = 0, llimit = 0;
	    dist->support(&llimit, &ulimit, 1, node->parameters(ch), 
			  node->parameterDims());
	    if (!jags_finite(ulimit) || !jags_finite(llimit))
		return false;

	    //We don't want too many possibilities
	    double n = ulimit - llimit + 1;
	    if (n <= 1 || n > 20) //fixme: totally arbitrary
		return false;

	    //Support must be fixed
	    vector<bool> fixmask(node->parents().size());
	    for (unsigned int i = 0; i < node->parents().size(); ++i) {
		fixmask[i] = node->parents()[i]->isObserved();
	    }
	    if (!node->distribution()->isSupportFixed(fixmask))
		return false;
	}
	return true;
    }

    Sampler *  FiniteFactory::makeSingletonSampler(StochasticNode *snode,
						   Graph const &graph) const
    {
	unsigned int nchain = snode->nchain();
	vector<DensityMethod*> methods(nchain, 0);
	for (unsigned int i = 0; i < nchain; ++i) {
	    methods[i] = new FiniteSampler;
	}
	
	vector<StochasticNode*> nodes(1, snode);
	return new ParallelDensitySampler(nodes, graph, methods);
    }
    
}
