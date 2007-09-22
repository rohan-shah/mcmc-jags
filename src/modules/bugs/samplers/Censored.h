#ifndef CENSORED_SAMPLER_H_
#define CENSORED_SAMPLER_H_

#include <sampler/Sampler.h>

/**
 *
 * A sampler for censored real-valued distributions.
 *
 */
class Censored : public Sampler {
    StochasticNode const *_snode;
    std::vector<std::vector<double const *> > _parameters;
public:
    Censored(StochasticNode *snode, Graph const &graph);
    ~Censored();

    static bool canSample(StochasticNode *snode, Graph const &graph);
    void update(std::vector<RNG *> const &rng);
    bool adaptOff();
};

#endif /* CONJUGATE_NORMAL_H_ */
