#ifndef CONJUGATE_SAMPLER_H_
#define CONJUGATE_SAMPLER_H_

#include "ConjugateMethod.h"
#include <sampler/Sampler.h>

namespace bugs {

class ConjugateSampler : public Sampler
{
    ConjugateMethod *_method;
    const unsigned int _nchain;
public:
    ConjugateSampler(GraphView *gv, ConjugateMethod *method);
    ~ConjugateSampler();
    void update(std::vector<RNG*> const &);
    /**
     * Conjugate methods are not adaptive
     */
    bool isAdaptive() const;
    bool checkAdaptation() const;
    void adaptOff();
    std::string name() const;
};

}

#endif /* CONJUGATE_SAMPLER_H_ */
