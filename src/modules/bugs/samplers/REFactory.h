#ifndef RANDOM_EFFECT_FACTORY_H_
#define RANDOM_EFFECT_FACTORY_H_

#include <sampler/SamplerFactory.h>
class StochasticNode;
class Graph;

namespace jags {
namespace bugs {

/**
 * @short Factory object for random effect samplers
 *
 * Random effects are defined as a set of N > 20 unobserved stochastic
 * nodes with mean 0 and a common variance. The RandomEffectSampler
 * updates the effects and their variance simultaneously.
 * 
 */
class REFactory : public SamplerFactory
{
    std::string _name;
public:
    REFactory();
    void makeSampler(std::set<StochasticNode*, less_sampler> &nodes, 
		     Graph const &graph,
		     std::vector<std::vector<Sampler*> > &samplers) const;
    std::string const &name() const;
};

}}

#endif /* RANDOM_EFFECT_FACTORY_H_ */
