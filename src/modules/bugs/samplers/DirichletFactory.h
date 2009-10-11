#ifndef DIRICHLET_FACTORY_H_
#define DIRICHLET_FACTORY_H_

#include "Dirichlet.h"

#include <sampler/SingletonFactory.h>

/**
 * @short Factory object for multivariate normal samplers
 */
class DirichletFactory : public SingletonFactory
{
    std::string _name;
public:
    DirichletFactory();
    bool canSample(StochasticNode *snode, Graph const &graph) const;
    Sampler *makeSampler(StochasticNode *snode, Graph const &graph) const;
    std::string const &name() const;
};

#endif /* DIRICHLET_FACTORY_H_ */
