#ifndef CONJUGATE_GAMMA_H_
#define CONJUGATE_GAMMA_H_

#include "ConjugateMethod.h"

namespace bugs {

class ConjugateGamma : public ConjugateMethod {
    double *_coef;
public:
    ConjugateGamma(GraphView const *gv);
    ~ConjugateGamma();
    static bool canSample(StochasticNode *snode, Graph const &graph);
    void update(unsigned int chain, RNG *rng) const;
    std::string name() const;
};

}

#endif /* CONJUGATE_GAMMA_H_ */
