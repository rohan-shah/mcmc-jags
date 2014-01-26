#ifndef CONJUGATE_GAMMA_H_
#define CONJUGATE_GAMMA_H_

#include "ConjugateMethod.h"

namespace jags {
    
    class Graph;

namespace bugs {

class ConjugateGamma : public ConjugateMethod {
    double *_coef;
public:
    ConjugateGamma(SingletonGraphView const *gv);
    ~ConjugateGamma();
    static bool canSample(StochasticNode *snode, Graph const &graph);
    void update(unsigned int chain, RNG *rng) const;
};

}}

#endif /* CONJUGATE_GAMMA_H_ */
