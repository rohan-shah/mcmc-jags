#ifndef CENSORED_H_
#define CENSORED_H_

#include "ConjugateMethod.h"

/**
 * Update Method for censored real-valued distributions.
 */
class Censored : public ConjugateMethod {
public:
    Censored(GraphView const *gv);
    static bool canSample(StochasticNode *snode, Graph const &graph);
    void update(unsigned int chain, RNG * rng) const;
    std::string name() const;
};

#endif /* CENSORED_H_ */
