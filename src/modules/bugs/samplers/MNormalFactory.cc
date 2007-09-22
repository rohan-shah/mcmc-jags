#include <config.h>

#include "MNormal.h"
#include "MNormalFactory.h"
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/ParallelDensitySampler.h>

#include <string>
#include <vector>

using std::vector;


bool 
MNormalFactory::canSample(StochasticNode * snode, Graph const &graph) const
{
    return snode->distribution()->name() == "dmnorm" && !isBounded(snode);
}

Sampler *
MNormalFactory::makeSingletonSampler(StochasticNode *snode, Graph const &graph)
				      const
{
    unsigned int nchain = snode->nchain();
    vector<DensityMethod*> methods(nchain, 0);
    vector<StochasticNode*> nodes(1, snode);
    for (unsigned int ch = 0; ch < nchain; ++ch) {
        methods[ch] = new MNormUpdate(nodes);
    }
    return new ParallelDensitySampler(nodes, graph, methods);
}
