#ifndef FINITE_SAMPLER_H_
#define FINITE_SAMPLER_H_

#include <sampler/Sampler.h>
class StochasticNode;

/**
 * Sampler for discrete distributions with support on a finite set.
 */
class FiniteSampler : public Sampler {
  const unsigned int _chain;
    int _lower, _upper;
public:
    FiniteSampler(StochasticNode *node, Graph const &graph, unsigned int chain);
    ~FiniteSampler();

    void update(RNG *rng);
    static bool canSample(StochasticNode const *node, Graph const &graph);
    AdaptStatus adaptOff();
};

#endif /* FINITE_SAMPLER_H_ */
