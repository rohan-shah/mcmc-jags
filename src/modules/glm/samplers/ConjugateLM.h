#ifndef CONJUGATE_LM_H_
#define CONJUGATE_LM_H_

#include <cs.h>
#include <string>

class Graph;
class RNG;
class LMSampler;

/**
 * Conjugate sampler for normal linear models.
 */
class ConjugateLM {
    cs *_betas;
    int _nrow, _ncol;
    void calBeta(cs *_betas, LMSampler *sampler, unsigned int chain);
public:
    ConjugateLM();
    ~ConjugateLM();
    void initialize(LMSampler *sampler, Graph const &graph);
    void update(LMSampler *sampler, unsigned int chain, RNG *rng) const;
    std::string name() const;
};

#endif /* CONJUGATE_LM_ */
