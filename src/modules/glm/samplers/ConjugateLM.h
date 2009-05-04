#ifndef CONJUGATE_LM_H_
#define CONJUGATE_LM_H_

#include <string>

class Graph;
class RNG;
class LMSampler;

/**
 * Conjugate sampler for normal linear models.
 */
class ConjugateLM {
    double *_betas;
    unsigned int _length_betas;
public:
    ConjugateLM();
    ~ConjugateLM();
    void initialize(LMSampler *sampler, Graph const &graph);
    void update(LMSampler *sampler, unsigned int chain, RNG *rng) const;
    std::string name() const;
};

#endif /* CONJUGATE_LM_ */
