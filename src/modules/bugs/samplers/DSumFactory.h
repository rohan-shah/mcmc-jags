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
    void makeSampler(std::set<StochasticNode*> &nodes, 
		     Graph const &graph,
		     std::vector<Sampler*> &samplers)
	const;
};

#endif /* DSUM_FACTORY_H_ */
