#ifndef CONJUGATE_BETA_H_
#define CONJUGATE_BETA_H_

#include "ConjugateMethod.h"

namespace bugs {

/**
 * @short Conjugate sampler for the beta distribution
 *
 * A conjugate beta sampler can be created for a Stochastic node
 * "snode" if its children within the graph are all stochastic nodes
 * with either a binomial or a Bernoulli distribution with snode as
 * the probability parameter. In the case of binomial children, the
 * sample size parameter must not depend on snode.
 */
class ConjugateBeta : public ConjugateMethod {
public:
    ConjugateBeta(GraphView const *gv);
    void update(unsigned int chain, RNG *rng) const;
    static bool canSample(StochasticNode *snode, Graph const &graph);
    std::string name() const;
};

}

#endif /* CONJUGATE_BETA_H_ */
