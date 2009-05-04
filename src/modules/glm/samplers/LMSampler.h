#ifndef LM_SAMPLER_H_
#define LM_SAMPLER_H_

#include <sampler/Sampler.h>
#include "ConjugateLM.h"

class LMSampler : public Sampler
{
    ConjugateLM _method;
public:
    LMSampler(std::vector<StochasticNode*> const &nodes, Graph const &graph);
    ~LMSampler();
    void update(std::vector<RNG*> const &);
    /**
     * Conjugate methods are not adaptive
     */
    bool isAdaptive() const;
    bool adaptOff();
    std::string name() const;
};

#endif /* LM_SAMPLER_H_ */
