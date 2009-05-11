#ifndef CONJUGATE_LM_H_
#define CONJUGATE_LM_H_

extern "C" {
#include <cs.h>
}
#include <string>

class Graph;
class RNG;
class LMSampler;

/**
 * Conjugate sampler for normal linear models.
 */
class ConjugateLM {
    cs *_beta;
    bool _fixed;
    unsigned int _length_max;
    unsigned _nz_prior;
    void calBeta(cs *_beta, LMSampler *sampler, unsigned int chain) const;
public:
    ConjugateLM();
    ~ConjugateLM();
    void initialize(LMSampler *sampler, Graph const &graph);
    void update(LMSampler *sampler, unsigned int chain, RNG *rng) const;
    std::string name() const;
};

#endif /* CONJUGATE_LM_ */
