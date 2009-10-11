#ifndef DSUM_FACTORY_H_
#define DSUM_FACTORY_H_

#include <sampler/SamplerFactory.h>

/**
 * @short Factory object for discrete sum samplers
 */
class DSumFactory : public SamplerFactory
{
    std::string _name;
public:
    DSumFactory();
    Sampler * makeSampler(std::set<StochasticNode*> const &nodes, 
			  Graph const &graph) const;
    std::string const &name() const;
};

#endif /* DSUM_FACTORY_H_ */
