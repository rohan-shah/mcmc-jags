#ifndef CENSORED_H_
#define CENSORED_H_

#include <sampler/DensityMethod.h>

class Updater;
class StochasticNode;
class Graph;

/**
 * Update Method for censored real-valued distributions.
 */
class Censored : public DensityMethod {
    Updater const *_updater;
    unsigned int _chain;
public:
    Censored(Updater const *updater, unsigned int chain);
    ~Censored();

    static bool canSample(StochasticNode *snode, Graph const &graph);
    void update(RNG * rng);
    bool isAdaptive() const;
    bool adaptOff();
    std::string name() const;
};

#endif /* CENSORED_H_ */
