#ifndef CENSORED_H_
#define CENSORED_H_

#include "ConjugateMethod.h"

/**
 * Update Method for censored real-valued distributions.
 */
class Censored : public ConjugateMethod {
public:
    Censored(Updater const *updater);
    static bool canSample(StochasticNode *snode, Graph const &graph);
    void update(Updater *updater, unsigned int chain, RNG * rng) const;
    std::string name() const;
};

#endif /* CENSORED_H_ */
