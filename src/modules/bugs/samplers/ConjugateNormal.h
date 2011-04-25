#ifndef CONJUGATE_NORMAL_H_
#define CONJUGATE_NORMAL_H_

#include "ConjugateMethod.h"

namespace bugs {

/**
 * Conjugate sampler for the normal distribution.
 * A conjugate sampler can be created for a stochastic node "snode"
 * if it has a normal distribution, all of its marginal stochastic
 * children are normal, they depend only on snode through the mean
 * (and not the precision) and the mean is a linear function of
 * snode.
 */
class ConjugateNormal : public ConjugateMethod {
    double *_betas;
    unsigned long _length_betas;
public:
    ConjugateNormal(GraphView const *gv);
    ~ConjugateNormal();
    void update(unsigned int chain, RNG *rng) const;
    static bool canSample(StochasticNode *snode, Graph const &graph);
    std::string name() const;
};

}

#endif /* CONJUGATE_NORMAL_H_ */
