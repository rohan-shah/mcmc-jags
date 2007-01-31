#ifndef MNORMAL_FACTORY_H_
#define MNORMAL_FACTORY_H_

#include "MNormal.h"

#include <sampler/GibbsFactory.h>
class StochasticNode;

/**
 * @short Factory object for multivariate normal samplers
 */
class MNormalFactory : public GibbsFactory
{
public:
    bool canSample(StochasticNode *snode, Graph const &graph) const;
    Sampler *makeGibbsSampler(StochasticNode *snode, Graph const &graph, 
			      unsigned int chain) const;
};

#endif /* MNORMAL_FACTORY_H_ */
