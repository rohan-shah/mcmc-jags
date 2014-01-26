#ifndef TRUNCATED_GAMMA_H_
#define TRUNCATED_GAMMA_H_

#include "ConjugateMethod.h"

namespace jags {

    class Graph;

namespace bugs {

class TruncatedGamma : public ConjugateMethod {
    const double _exponent;
public:
    TruncatedGamma(SingletonGraphView const *gv);
    ~TruncatedGamma();
    static bool canSample(StochasticNode *snode, Graph const &graph);
    void update(unsigned int chain, RNG *rng) const;
};

}}

#endif /* TRUNCATED_GAMMA_H_ */
