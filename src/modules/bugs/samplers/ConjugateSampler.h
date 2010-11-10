#ifndef CONJUGATE_SAMPLER_H_
#define CONJUGATE_SAMPLER_H_

#include "ConjugateMethod.h"
#include <sampler/Sampler.h>

class ConjugateSampler : public Sampler
{
    ConjugateMethod *_method;
    const unsigned int _nchain;
public:
    ConjugateSampler(GraphView *gv, ConjugateMethod *method);
    ~ConjugateSampler();
    bool update(std::vector<RNG*> const &);
    /**
     * Conjugate methods are not adaptive
     */
    bool isAdaptive() const;
    bool adaptOff();
    std::string name() const;
};

#endif /* CONJUGATE_SAMPLER_H_ */
