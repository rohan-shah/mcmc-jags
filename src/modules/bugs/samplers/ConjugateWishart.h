#ifndef CONJUGATE_WISHART_H_
#define CONJUGATE_WISHART_H_

#include "ConjugateMethod.h"

namespace bugs {

/**
 * Conjugate method for the Wishart distribution
 *
 * A Conjugate Wishart method for stochastic node "snode" can be
 * created if snode has a Wishart distribution and its stochastic
 * children are multivariate normal with precision matrix snode. The
 * mean of the children may not depend on snode.
 */
class ConjugateWishart : public ConjugateMethod {
public:
    ConjugateWishart(GraphView const *gv);
    void update(unsigned int chain, RNG *rng) const;
    static bool canSample(StochasticNode *snode, Graph const &graph);
    std::string name() const;
};

}

#endif /* CONJUGATE_WISHART_H_ */
