#ifndef CONJUGATE_DIAG_M_NORMAL_H_
#define CONJUGATE_DIAG_M_NORMAL_H_

#include "ConjugateMethod.h"

/**
 * Special conjugate sampler for multivariate normal distribution
 * in the case where all stochastic children are scalars with a 
 * normal distribution, and for which the coefficient matrix is
 * diagonal.  The usual conjugate normal sampler scales badly in
 * this situation
 */
class ConjugateDiagMNormal : public ConjugateMethod {
    vector<unsigned int> _offsets;
public:
    ConjugateDiagMNormal();
    ~ConjugateDiagMNormal();
    void initialize(ConjugateSampler *sampler, Graph const &graph);
    void update(ConjugateSampler *sampler, unsigned int chain, RNG *rng) const;
    static bool canSample(StochasticNode *snode, Graph const &graph);
    std::string name() const;
};

#endif /* CONJUGATE_DIAG_M_NORMAL_H_ */
