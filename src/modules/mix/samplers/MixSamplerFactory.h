#ifndef CONJUGATE_FACTORY_H_
#define CONJUGATE_FACTORY_H_

#include "MixSampler.h"

#include <sampler/GibbsFactory.h>
#include <map>
#include <string>
class StochasticNode;


/**
 * @short Factory object for mixture samplers
 */
class MixSamplerFactory : public GibbsFactory
{
public:
    MixSamplerFactory();
    void makeSampler(std::set<StochasticNode*> &nodes, 
		     Graph const &graph,
		     std::vector<std::vector<Sampler*> > &samplers) const;
};

#endif /* CONJUGATE_FACTORY_H_ */
