#ifndef DSUM_FACTORY_H_
#define DSUM_FACTORY_H_

#include <sampler/SamplerFactory.h>

/**
 * @short Factory object for discrete sum samplers
 */
class DSumFactory : public SamplerFactory
{
 public:
    Sampler * makeSampler(std::set<StochasticNode*> const &nodes, 
			  Graph const &graph) const;
};

#endif /* DSUM_FACTORY_H_ */
