#ifndef CONJUGATE_GAMMA_H_
#define CONJUGATE_GAMMA_H_

#include "ConjugateMethod.h"

class StochasticNode;
class Graph;

class ConjugateGamma : public ConjugateMethod {
    double *_coef;
public:
    ConjugateGamma();
    ~ConjugateGamma();
    static bool canSample(StochasticNode *snode, Graph const &graph);
    void update(ConjugateSampler *sampler, unsigned int chain, RNG *rng) const;
    void initialize(ConjugateSampler *sampler);
};

#endif /* CONJUGATE_GAMMA_H_ */
