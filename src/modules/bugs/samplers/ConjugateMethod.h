#ifndef CONJUGATE_METHOD_H_
#define CONJUGATE_METHOD_H_

#include <sampler/ImmutableSampleMethod.h>

#include <vector>

namespace jags {

class SingletonGraphView;
class StochasticNode;
struct RNG;

namespace bugs {

enum ConjugateDist {
    BERN, BETA, BIN, CAT, CHISQ, DEXP, DIRCH, EXP, GAMMA,
    LNORM, LOGIS, MNORM, MULTI, NEGBIN, NORM, PAR, POIS,
    T, UNIF, WEIB, WISH, OTHERDIST
};

/**
 * Convenience function that converts the name of a distribution into
 * a member of the ConjugateDist enumeration. OTHERDIST is returned
 * if the name cannot be found in the internal table.
 */
ConjugateDist getDist(StochasticNode const *snode);

class ConjugateMethod : public ImmutableSampleMethod
{
protected:
    const ConjugateDist _target_dist;
    const std::vector<ConjugateDist> _child_dist;
    SingletonGraphView const * const _gv;
public:
    ConjugateMethod(SingletonGraphView const *gv);
};

}}

#endif /* CONJUGATE_METHOD_H_ */

