#ifndef CONJUGATE_FACTORY_H_
#define CONJUGATE_FACTORY_H_

#include "MixSampler.h"

#include <sampler/Sampler.h>
#include <map>
#include <string>
class StochasticNode;


/**
 * @short Factory object for conjugate samplers
 */
class MixSamplerFactory : public GibbsFactory
{
public:
    MixSamplerFactory();
    bool canSample(StochasticNode *snode, Graph const &graph) const;
    void makeSampler(std::set<StochasticNode*> &nodes, 
		     Graph const &graph,
		     std::vector<std::vector<Sampler*> > &samplers) const;
};

#endif /* CONJUGATE_FACTORY_H_ */
