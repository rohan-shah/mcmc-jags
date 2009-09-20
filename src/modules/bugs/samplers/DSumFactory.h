#ifndef DSUM_FACTORY_H_
#define DSUM_FACTORY_H_

#include <sampler/SamplerFactory.h>

class StochasticNode;
class Graph;

/**
 * @short Factory object for discrete sum samplers
 */
class DSumFactory : public SamplerFactory
{
 public:
    Sampler * makeSampler(StochasticNodeSet const &nodes, 
		     Graph const &graph) const;
};

#endif /* DSUM_FACTORY_H_ */
