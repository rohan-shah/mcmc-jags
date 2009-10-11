#ifndef MNORMAL_FACTORY_H_
#define MNORMAL_FACTORY_H_

#include "MNormal.h"

#include <sampler/SingletonFactory.h>

/**
 * @short Factory object for multivariate normal samplers
 */
class MNormalFactory : public SingletonFactory
{
    std::string _name;
public:
    MNormalFactory();
    bool canSample(StochasticNode *snode, Graph const &graph) const;
    Sampler *makeSampler(StochasticNode *snode, Graph const &graph) const;
    std::string const &name() const;
};

#endif /* MNORMAL_FACTORY_H_ */
