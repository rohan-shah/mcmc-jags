#ifndef CONJUGATE_GAMMA_H_
#define CONJUGATE_GAMMA_H_

#include "ConjugateMethod.h"

class ConjugateGamma : public ConjugateMethod {
    double *_coef;
public:
    ConjugateGamma(Updater const *updater);
    ~ConjugateGamma();
    static bool canSample(StochasticNode *snode, Graph const &graph);
    void update(Updater *updater, unsigned int chain, RNG *rng) const;
    std::string name() const;
};

#endif /* CONJUGATE_GAMMA_H_ */
