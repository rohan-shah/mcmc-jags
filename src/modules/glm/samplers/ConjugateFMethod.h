#ifndef CONJUGATE_F_METHOD_H_
#define CONJUGATE_F_METHOD_H_

#include <sampler/SampleMethod.h>
#include <sampler/GraphView.h>

/**
 * When an F distribution is used as a prior for the precision of
 * normally distributed random effects, it has a conditionally
 * conjugate distribution using a redundant parametrization.
 */
class ConjugateFMethod {
    GraphView *_gv1, *_gv2;
    unsigned int _chain;
    double _scale, _tau;
    double *_coef;
public:
    ConjugateFMethod(GraphView *gv1, GraphView *gv2, unsigned int chain);
    ~ConjugateFMethod();
    void update(RNG *rng);
    static bool canSample(StochasticNode *snode, Graph const &graph);
};

#endif /* CONJUGATE_F_METHOD_H_ */
