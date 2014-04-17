#ifndef DSUM_FACTORY_H_
#define DSUM_FACTORY_H_

#include <sampler/SamplerFactory.h>

namespace jags {
namespace bugs {

/**
 * @short Factory object for discrete sum samplers
 */
class DSumFactory : public SamplerFactory
{
public:
    std::vector<Sampler*> makeSamplers(std::list<StochasticNode*> const &nodes, 
				       Graph const &graph) const;
    Sampler * makeSampler(std::list<StochasticNode*> const &nodes, 
			  Graph const &graph) const;
    std::string name() const;
};

}}

#endif /* DSUM_FACTORY_H_ */
