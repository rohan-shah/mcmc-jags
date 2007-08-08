#include <config.h>

#include "MNormal.h"
#include "MNormalFactory.h"
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

#include <string>


bool 
MNormalFactory::canSample(StochasticNode * snode, Graph const &graph) const
{
    return snode->distribution()->name() == "dmnorm" && !snode->isBounded();
}

Sampler *
MNormalFactory::makeSingletonSampler(StochasticNode *snode, Graph const &graph,
				     unsigned int chain) const
{
    return new MNormSampler(snode, graph, chain, snode->value(chain), 
			      snode->length());
}
