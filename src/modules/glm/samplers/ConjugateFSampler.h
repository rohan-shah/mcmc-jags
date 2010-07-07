#ifndef CONJUGATE_F_SAMPLER_H_
#define CONJUGATE_F_SAMPLER_H_

#include <sampler/Sampler.h>

class ConjugateFMethod;

/**
 * The Conjugate F method requires two GraphView objects, instead of
 * the usual one GraphView.
 */
class ConjugateFSampler : public Sampler
{
    GraphView *_gv2;
    std::vector<ConjugateFMethod*> _methods;
public:
    ConjugateFSampler(GraphView *gv1, GraphView *gv2, 
		      std::vector<ConjugateFMethod*> const &methods);
    ~ConjugateFSampler();
    void update(std::vector<RNG*> const &rngs);
    bool isAdaptive() const;
    bool adaptOff();
    std::string name() const;
};

#endif /* CONJUGATE_F_SAMPLER_H_ */
