#ifndef CONJUGATE_METHOD_H_
#define CONJUGATE_METHOD_H_

#include <string>
#include <vector>

class Updater;
class RNG;
class Graph;
class StochasticNode;

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

class ConjugateMethod
{
protected:
    const ConjugateDist _target_dist;
    const std::vector<ConjugateDist> _child_dist;
public:
    ConjugateMethod(Updater const *updater);
    virtual ~ConjugateMethod();
    virtual void update(Updater *updater, unsigned int chain, RNG *rng) 
	const = 0;
    virtual std::string name() const = 0;
};

#endif /* CONJUGATE_METHOD_H_ */
