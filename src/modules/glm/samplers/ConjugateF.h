#ifndef CONJUGATE_F_H_
#define CONJUGATE_F_H_

#include "SampleMethod.h"

class ConjugateF : public SampleMethod {
    GraphView _gv1, _gv2;
    unsigned int _chain;
    double scale, tau0;
    double *_coef;
public:
    ConjugateF(GraphView const *gv, unsigned int chain);
    ~ConjugateF();
    void update(RNG *rng);
    bool isAdaptive() const;
    bool adaptOff();
    std::string name() const;
    static bool canSample(StochasticNode *snode, Graph const &graph);
};

#endif /* CONJUGATE_F_H_ */
