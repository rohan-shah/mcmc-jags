#ifndef CONJUGATE_SAMPLER_H_
#define CONJUGATE_SAMPLER_H_

#include "ConjugateMethod.h"
#include <sampler/Sampler.h>

class ConjugateSampler : public Sampler
{
    GraphView *_gv;
    ConjugateMethod *_method;
    const ConjugateDist _target_dist;
    const std::vector<ConjugateDist> _child_dist;
public:
    ConjugateSampler(GraphView *gv, ConjugateMethod *method);
    ~ConjugateSampler();
    void update(std::vector<RNG*> const &);
    /**
     * Conjugate methods are not adaptive
     */
    bool isAdaptive() const;
    bool adaptOff();
    std::string name() const;
};

#endif /* CONJUGATE_SAMPLER_H_ */
